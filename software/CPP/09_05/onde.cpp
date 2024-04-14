#include "onde.h"
#include "constantes.h"
#include <algorithm>
#include "formule.h"


using namespace std::complex_literals;

//calcule le champs reçu en chemin direct sans obstacle 
std::complex<float> champIncident(emetteur Source, point Recepteur, float d) {
    float GTX = calculGTX(Source, Recepteur, 3);
    float PTX = Source.PTX; 
    float omega = Source.freq * 2 * M_PI;
    float beta = omega / c;
    std::complex<float> expo= std::exp(- beta * d * 1if);   
    std::complex<float> Ei = sqrt(60.F * GTX * PTX) * expo / d;  //attention changer PTX*GTX
    return Ei;
}

float puissanceDirect(emetteur Source, point Recepteur) {
    float he = Source.he;
    float d = distance(Source.coordonnees, Recepteur); 
    float Ecarre = std::norm(champIncident(Source, Recepteur, d));
    float Puissance = (1 / (8 * Source.resistance)) * Ecarre * pow(he, 2); 
    return Puissance;
}

//calcule le champ de tranmission en prenant tous les obstacle en compte
std::complex<float> champTransTotal(const emetteur& Source, const point& Recepteur, const std::vector<mur>& MursTrans) {
  
    const point pSource = Source.coordonnees;
    const float d = distance(pSource, Recepteur);
    const float omega = Source.freq * 2 * M_PI;
    std::complex<float> Tm = 1; 
    for (const mur& murTrans : MursTrans) {
        std::complex<float> zmT = calcul_zm(Source, murTrans);
        std::complex<float> gmT = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
        Tm *= coeffTrans(murTrans, Recepteur, Source, pSource , zmT, gmT);    
        //std::cout << "coeff de transmission: " << Tm << std::endl;
    }
    std::complex<float> Etot = Tm * champIncident(Source, Recepteur, d);
    return Etot; 
}

//calcule le champs de la réflexion sans obstacle
std::complex<float> champReflex(const emetteur& Source, const point& Recepteur, const mur& MurReflexion) {
   
    const point pSource = Source.coordonnees;
    const point I1 = trouverSym(pSource, MurReflexion); 
    const float dr = distance(I1, Recepteur); 
    const float epsilon = MurReflexion.permittivite;
    const float sigma = MurReflexion.conductivite;
    const float omega = Source.freq * 2 * M_PI;
 
    std::complex<float> gm = calcul_gm(epsilon, sigma, omega);
    std::complex<float> Zm = calcul_zm(Source, MurReflexion);
    std::complex<float> Rm = coeffReflex(MurReflexion, Recepteur, Source, pSource, Zm, gm, I1);

    std::complex<float> EReflex = Rm * champIncident(Source, Recepteur, dr); 
    return EReflex; 
}

//calcule le champs de reflexion en prenant en compte les obstacles
std::complex<float> champReflexTotal(const emetteur& Source, const point& Recepteur, const point& PtReflexion,
    const mur& MurReflex, const std::vector<mur>& MursTrans1, const std::vector<mur>& MursTrans2){

    float omega = 2 * M_PI * Source.freq; 
    float resistanceSource = Source.resistance;
    float he = Source.he;
    point pSource = Source.coordonnees;
    
    std::complex<float> EReflex = champReflex(Source, Recepteur, MurReflex);
    std::complex<float> Tm1 = coeffTransTotal(Source, PtReflexion, pSource, omega, MursTrans1); 
    std::complex<float> Tm2 = coeffTransTotal(Source, Recepteur, PtReflexion, omega, MursTrans2);

    std::complex<float> Etot = EReflex * Tm1 * Tm2;
    return Etot;

}


//calcule le champ de double reflexion sans obstacle
std::complex<float> champDoubleReflex(const emetteur& Source, const point& Recepteur, const mur& MurReflex1,
    const mur& MurReflex2, const point& I1, const point& I2, const point& PtReflexion2) {

    const point pSource = Source.coordonnees;
    const float epsilon1 = MurReflex1.permittivite;
    const float epsilon2 = MurReflex2.permittivite; 
    const float sigma1 = MurReflex1.conductivite;
    const float sigma2 = MurReflex2.conductivite; 
    const float omega = Source.freq * 2 * M_PI;
    const float drr = distance(I2, Recepteur);

    //std::cout << "    " << omega << std::endl; 

    std::complex<float> gm1 = calcul_gm(epsilon1, sigma1, omega);
    std::complex<float> Zm1 = calcul_zm(Source, MurReflex1);
    std::complex<float> Rm1 = coeffReflex(MurReflex1, PtReflexion2, Source, pSource, Zm1, gm1, I1);  //reflexion de source vers ptReflexion2
    
    std::complex<float> gm2 = calcul_gm(epsilon2, sigma2, omega);
    std::complex<float> Zm2 = calcul_zm(Source, MurReflex2);
    std::complex<float> Rm2 = coeffReflex(MurReflex2, Recepteur, Source, I1, Zm2, gm2, I2);          //reflexion de I vers recepteur 
 
    std::complex<float> EDoubleReflex = Rm1 * Rm2 * champIncident(Source, Recepteur, drr);
    return EDoubleReflex;

}



/// <summary>
/// 
/// </summary>
/// <param name="Source"></param>
/// <param name="Recepteur"></param>
/// <param name="I1"></param>
/// <param name="I2"></param>
/// <param name="PtReflexion1"></param>
/// <param name="PtReflexion2"></param>
/// <param name="DeuxMursReflex"></param>
/// <param name="MursTrans1"></param>
/// <param name="MursTrans2"></param>
/// <param name="MursTrans3"></param>
/// <returns></returns>
std::complex<float> champDoubleReflexTotal(
    const emetteur& Source, const point& Recepteur,
    const point& I1, const point& I2, const point& PtReflexion1, const point& PtReflexion2,
    const std::vector<mur>& DeuxMursReflex,
    const std::vector<mur>& MursTrans1, const std::vector<mur>& MursTrans2, const std::vector<mur>& MursTrans3){

    const float omega = 2 * M_PI * Source.freq;
    const point pSource = Source.coordonnees;
    const std::complex<float> EDoubleReflex = champDoubleReflex(Source, Recepteur, DeuxMursReflex[0], DeuxMursReflex[1], I1, I2, PtReflexion2);

    std::complex<float> Tm1 = coeffTransTotal(Source, PtReflexion1, pSource, omega, MursTrans1);
    //std::cout << "coeff de transmission1: " << Tm1 << std::endl;

    std::complex<float> Tm2 = coeffTransTotal(Source, PtReflexion2, PtReflexion1, omega, MursTrans2); 
    //std::cout << "coeff de transmission2: " << Tm2 << std::endl;
   
    std::complex<float> Tm3 = coeffTransTotal(Source, Recepteur, PtReflexion2, omega, MursTrans3); 
    //std::cout << "coeff de transmission3: " << Tm3 << std::endl; 

    //std::cout << std::endl << EDoubleReflex * Tm1 * Tm2 * Tm3 << std::endl; 
    return EDoubleReflex * Tm1 * Tm2 * Tm3;
}