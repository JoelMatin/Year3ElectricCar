#include "onde.h"
#include "constantes.h"
#include <algorithm>
#include "formule.h"

using namespace std::complex_literals;

//calcule le champs reçu en chemin direct
std::complex<float> champIncident(emetteur Source, point Recepteur, float d) {
    float GTX = calculGTX(Source, Recepteur);
    float omega = Source.freq * 2 * M_PI;
    float beta = omega / c;
    std::complex<float> expo= std::exp(- beta * d * 1if);   
    std::complex<float> Ei = sqrt(60.F * 1.64F * (float)pow(10, -3)) * expo / d;  //attention changer PTX*GTX
    return Ei;
}

// Puissance pour chemin direct
float puissanceDirect(emetteur Source, point Recepteur) {
    float he = Source.he;
    float d = distance(Source.coordonnees, Recepteur); 
    float Ecarre = std::norm(champIncident(Source, Recepteur, d));
    float Puissance = (1 / (8 * Source.resistance)) * Ecarre * pow(he, 2);  // !!!!!!!! ATTENTION !!!!!! FAUT VERIFEIR CA JFAIS UNE PTITE COUILLE avec les exponentielles
    return Puissance;
}


std::complex<float> champTransmission(emetteur Source, point Recepteur, mur MurTransmission) {
    float permitivite, sigma, omega, beta, d;
    point pSource = Source.coordonnees;
    d = distance(pSource, Recepteur);
    permitivite = MurTransmission.permittivite;
    sigma = MurTransmission.conductivite;
    omega = Source.freq * 2 * M_PI;
    beta = omega / c;
    std::complex<float> gm = calcul_gm(MurTransmission.permittivite, sigma, omega);
    std::complex<float> Zm = calcul_zm(Source, MurTransmission);
    std::complex<float> Tm = coeffTrans(MurTransmission, Recepteur, Source, Source.coordonnees, Zm, gm);
    std::complex<float> ETrans = Tm * champIncident(Source, Recepteur, d);
    return ETrans; 

}
// Puissance par transmission
float puissanceTransmission(emetteur Source, point Recepteur, mur MurTransmission) {
    float he = Source.he;
    std::complex<float> ETrans = champTransmission(Source, Recepteur, MurTransmission); 
    float PTrans = 1 / (8 * Source.resistance) * std::norm(ETrans) * (float)pow(he, 2);       // std::norm donne la norme au carré du complexe 
    return PTrans;
}

//calcule le champs de la réflexion en supposant qu'on connait le mûr sur lequel elle a lieu
std::complex<float> champReflexion(emetteur Source, point Recepteur, mur MurReflexion) {
    float epsilon, sigma, omega, beta, dr;

    point pSource = Source.coordonnees;
    point I = trouverSym(pSource, MurReflexion); 
    dr = distance(I, Recepteur); 
    epsilon = MurReflexion.permittivite;
    sigma = MurReflexion.conductivite;
    omega = Source.freq * 2 * M_PI;
    beta = omega / c;

    std::complex<float> gm = calcul_gm(epsilon, sigma, omega);
    std::complex<float> Zm = calcul_zm(Source, MurReflexion);
    std::complex<float> Rm = coeffReflex(MurReflexion, Recepteur, Source, pSource, Zm, gm, I);

    std::complex<float> EReflex = Rm * champIncident(Source, Recepteur, dr); 
    return EReflex; 
}


float puissanceReflexion(emetteur Source, point Recepteur, mur MurReflex, std::vector<mur> MursTrans1, std::vector<mur> MursTrans2) {
    float omega = 2 * M_PI * Source.freq; 
    float resistanceSource = Source.resistance;
    float he = Source.he;
    point pSource = Source.coordonnees;
    point PtReflexion = calculPtReflexion(Source.coordonnees, Recepteur, MurReflex); 
    
    std::complex<float> EReflex = champReflexion(Source, Recepteur, MurReflex);
    std::complex<float> Tm1 = 1;   //Tmtotal = Tm1*Tm2 avec Tm2 = Tm2_1*Tm2_2....Tm2_n si on passe par n murs entre le point de réflexion et le recepteur 
    std::complex<float> Tm2 = 1; 

    if (MursTrans1.empty()) {
        if (MursTrans2.empty()) {
            float PReflex = 1 / (8 * Source.resistance) * std::norm(EReflex) * (float)pow(he, 2);
            return PReflex;
        }
        else {
            for (const mur& murTrans : MursTrans2) {
                std::complex<float> zmT2 = calcul_zm(Source, murTrans);
                std::complex<float> gmT2 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                Tm2 *= coeffTrans(murTrans, Recepteur, Source, PtReflexion, zmT2, gmT2);    //PtReflexion à la place de source car c'est le point émetteur 
                std::cout << "coeff de transmission2: " << Tm2 << std::endl;
            }
        }
    }
    else {
        if (MursTrans2.empty()) {
            for (const mur& murTrans : MursTrans1) {
                std::complex<float> zmT1 = calcul_zm(Source, murTrans);
                std::complex<float> gmT1 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                Tm2 *= coeffTrans(murTrans, PtReflexion, Source, pSource, zmT1, gmT1);//PtReflexion à la place de recepteur car c'est le point cible 
                std::cout << "coeff de transmission1: " << Tm1 << std::endl;
            }
        }
        //si on a des murs sur les deux chemins
        else {
            for (const mur& murTrans : MursTrans1) {
                std::complex<float> zmT1 = calcul_zm(Source, murTrans);
                std::complex<float> gmT1 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                Tm2 *= coeffTrans(murTrans, PtReflexion, Source, pSource, zmT1, gmT1);
                std::cout << "coeff de transmission1: " << Tm1 << std::endl;
            }
            for (const mur& murTrans : MursTrans2) {
                std::complex<float> zmT2 = calcul_zm(Source, murTrans);
                std::complex<float> gmT2 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                Tm2 *= coeffTrans(murTrans, Recepteur, Source, PtReflexion, zmT2, gmT2);
                std::cout << "coeff de transmission2: " << Tm2 << std::endl;
            }
        }
    }
    
    std::complex<float> Etotal = EReflex * Tm1 * Tm2;
    std::cout << "Champ quand y a transmission : " << Etotal << std::endl;

    //
    std::complex<float> Et = champTransmission(Source, Recepteur, MursTrans1[0]);
    std::cout << "Et: " << Et << std::endl;
    float Ptrans = puissanceTransmission(Source, Recepteur, MursTrans1[0]);
    std::cout << "puissance de transmission directe: " << Ptrans << std::endl;

    float somme = std::norm(Et + Etotal); 
    float Ptotale = 1 /( 8*resistanceSource) * (float)pow(he, 2) * somme; 
    std::cout << "Ptotale: " << Ptotale << std::endl;

    return Ptotale;
}


std::complex<float> champDoubleReflexion(emetteur Source, point Recepteur, mur MurReflex1,
    mur MurReflex2, point& I1, point& I2, point& PtReflexion2) {
    float epsilon1, epsilon2, sigma1, sigma2, omega, beta, drr;

    point pSource = Source.coordonnees;
    epsilon1 = MurReflex1.permittivite;
    epsilon2 = MurReflex2.permittivite; 
    sigma1 = MurReflex1.conductivite;
    sigma2 = MurReflex2.conductivite; 
    omega = Source.freq * 2 * M_PI;
    beta = omega / c;

    I1 = trouverSym(pSource, MurReflex1); 
    I2 = trouverSym(I1, MurReflex2); 
    drr = distance(I2, Recepteur); 
    PtReflexion2 = calculPtReflexion(I1, Recepteur, MurReflex2);     //pas besoin de calculer ptReflexion1

    std::complex<float> gm1 = calcul_gm(epsilon1, sigma1, omega);
    std::complex<float> Zm1 = calcul_zm(Source, MurReflex1);
    std::complex<float> Rm1 = coeffReflex(MurReflex1, PtReflexion2, Source, pSource, Zm1, gm1, I1);  //reflexion de source vers ptReflexion2
    
    std::complex<float> gm2 = calcul_gm(epsilon2, sigma2, omega);
    std::complex<float> Zm2 = calcul_zm(Source, MurReflex2);
    std::complex<float> Rm2 = coeffReflex(MurReflex2, Recepteur, Source, I1, Zm1, gm1, I2);          //reflexion de I vers recepteur 

    std::complex<float> EDoubleReflex = Rm1 * Rm2 * champIncident(Source, Recepteur, drr);
    return EDoubleReflex;

}


/// <summary>: calcule la puissance en un point due à une double réflexion 
/// <param name="MursReflex"></param>: vecteur contenant les deux murs
/// <param name="MursTrans1"></param>: vecteur des murs entre source et premier mur 
/// <param name="MursTrans2"></param>: vecteur des murs entre 1er mur et 2eme 
/// <param name="MursTrans3"></param>: vecteur des murs entre 2eme et recepteur
/// <returns></returns>

float puissanceDoubleReflexion(emetteur Source, point Recepteur, std::vector<mur>& MursReflex,
    std::vector<mur> MursTrans1, std::vector<mur> MursTrans2, std::vector<mur> MursTrans3) {
    float omega = 2 * M_PI * Source.freq;
    float resistanceSource = Source.resistance;
    float he = Source.he;
    point pSource = Source.coordonnees;

    point I1{};
    point I2{};
    point PtReflexion2{};
    std::complex<float> EDoubleReflex = champDoubleReflexion(Source, Recepteur, MursReflex[0],
        MursReflex[1], I1, I2, PtReflexion2);
    std::complex<float> Tm1 = 1;
    std::complex<float> Tm2 = 1;
    std::complex<float> Tm3 = 1;
    point PtReflexion1 = calculPtReflexion(pSource, PtReflexion2, MursReflex[0]);

    if (MursTrans1.empty()) {
        if (MursTrans2.empty()) {
            if (MursTrans3.empty()) {
                float PDoubleReflex = 1 / (8 * Source.resistance) * std::norm(EDoubleReflex) * (float)pow(he, 2);
                return PDoubleReflex;
            }
            else {
                for (const mur& murTrans : MursTrans3) {
                    std::complex<float> zmT3 = calcul_zm(Source, murTrans);
                    std::complex<float> gmT3 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                    Tm3 *= coeffTrans(murTrans, Recepteur, Source, PtReflexion2, zmT3, gmT3);
                    std::cout << "coeff de transmission3: " << Tm3 << std::endl;
                }
            }
        }
        else {
            if (MursTrans3.empty()) {
                for (const mur& murTrans : MursTrans2) {
                    std::complex<float> zmT2 = calcul_zm(Source, murTrans);
                    std::complex<float> gmT2 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                    Tm2 *= coeffTrans(murTrans, PtReflexion2, Source, PtReflexion1, zmT2, gmT2);
                    std::cout << "coeff de transmission2: " << Tm2 << std::endl;
                }
            }
            else {
                for (const mur& murTrans : MursTrans2) {
                    std::complex<float> zmT2 = calcul_zm(Source, murTrans);
                    std::complex<float> gmT2 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                    Tm2 *= coeffTrans(murTrans, PtReflexion2, Source, PtReflexion1, zmT2, gmT2);
                    std::cout << "coeff de transmission2: " << Tm2 << std::endl;
                }
                for (const mur& murTrans : MursTrans3) {
                    std::complex<float> zmT3 = calcul_zm(Source, murTrans);
                    std::complex<float> gmT3 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                    Tm3 *= coeffTrans(murTrans, Recepteur, Source, PtReflexion2, zmT3, gmT3);
                    std::cout << "coeff de transmission3: " << Tm3 << std::endl;
                }
            }
        }
    }
    else {
        if (MursTrans2.empty()) {
            if (MursTrans3.empty()) {
                for (const mur& murTrans : MursTrans1) {
                    std::complex<float> zmT1 = calcul_zm(Source, murTrans);
                    std::complex<float> gmT1 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                    Tm1 *= coeffTrans(murTrans, PtReflexion1, Source, pSource, zmT1, gmT1);
                    std::cout << "coeff de transmission1: " << Tm1 << std::endl;
                }
            }
            else {
                for (const mur& murTrans : MursTrans1) {
                    std::complex<float> zmT1 = calcul_zm(Source, murTrans);
                    std::complex<float> gmT1 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                    Tm1 *= coeffTrans(murTrans, PtReflexion1, Source, pSource, zmT1, gmT1);
                    std::cout << "coeff de transmission1: " << Tm1 << std::endl;
                }
                for (const mur& murTrans : MursTrans3) {
                    std::complex<float> zmT3 = calcul_zm(Source, murTrans);
                    std::complex<float> gmT3 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                    Tm3 *= coeffTrans(murTrans, Recepteur, Source, PtReflexion2, zmT3, gmT3);
                    std::cout << "coeff de transmission3: " << Tm3 << std::endl;
                }
            }
        }
        else {
            if (MursTrans3.empty()) {
                for (const mur& murTrans : MursTrans1) {
                    std::complex<float> zmT1 = calcul_zm(Source, murTrans);
                    std::complex<float> gmT1 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                    Tm1 *= coeffTrans(murTrans, PtReflexion1, Source, pSource, zmT1, gmT1);
                    std::cout << "coeff de transmission1: " << Tm1 << std::endl;
                }
                for (const mur& murTrans : MursTrans2) {
                    std::complex<float> zmT2 = calcul_zm(Source, murTrans);
                    std::complex<float> gmT2 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                    Tm2 *= coeffTrans(murTrans, PtReflexion2, Source, PtReflexion1, zmT2, gmT2);
                    std::cout << "coeff de transmission2: " << Tm2 << std::endl;
                }
            }
            else {
                for (const mur& murTrans : MursTrans1) {
                    std::complex<float> zmT1 = calcul_zm(Source, murTrans);
                    std::complex<float> gmT1 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                    Tm1 *= coeffTrans(murTrans, PtReflexion1, Source, PtReflexion1, zmT1, gmT1);
                    std::cout << "coeff de transmission1: " << Tm1 << std::endl;
                }
                for (const mur& murTrans : MursTrans2) {
                    std::complex<float> zmT2 = calcul_zm(Source, murTrans);
                    std::complex<float> gmT2 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                    Tm2 *= coeffTrans(murTrans, PtReflexion2, Source, PtReflexion1, zmT2, gmT2);
                    std::cout << "coeff de transmission2: " << Tm2 << std::endl;
                }
                for (const mur& murTrans : MursTrans3) {
                    std::complex<float> zmT3 = calcul_zm(Source, murTrans);
                    std::complex<float> gmT3 = calcul_gm(murTrans.permittivite, murTrans.conductivite, omega);
                    Tm3 *= coeffTrans(murTrans, Recepteur, Source, PtReflexion2, zmT3, gmT3);
                    std::cout << "coeff de transmission3: " << Tm3 << std::endl;
                }
            }
        }

        
    }
    std::complex<float> Etotal = EDoubleReflex * Tm1 * Tm2;
    float Ptotale = 1 / (8 * resistanceSource) * (float)pow(he, 2) * std::norm(Etotal);
    return Ptotale; 
}
