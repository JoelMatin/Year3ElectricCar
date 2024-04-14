#include "onde.h"
#include "constantes.h"
#include <algorithm>

using namespace std::complex_literals;

float distance(point p1, point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

float calculNorme(std::vector<float> unVecteur) {
    return sqrt(inner_product(unVecteur.begin(), unVecteur.end(), unVecteur.begin(), 0.0));
}

void vecteurNorme(std::vector<float>& unVecteur) {
    float norme = calculNorme(unVecteur);
    unVecteur = { unVecteur[0] / norme, unVecteur[1] / norme };
}

std::vector<float> additionerVecteur(std::vector<float> v1, std::vector<float> v2) {
    std::vector<float> v3 = { v1[0] + v2[0], v1[1] + v2[1] };
    return v3;
}

std::vector<float> soustraireVecteur(std::vector<float> v1, std::vector<float> v2) {
    std::vector<float> v3 = { v1[0] - v2[0], v1[1] - v2[1] };
    return v3;
}

std::vector<float> multiplierVecteur(std::vector<float> unVecteur, float scalaire) {
    std::vector<float> v = { unVecteur[0] * scalaire, unVecteur[1] * scalaire };
    return v;
}


/////////////////////////////////////////////////


//calcul de gamma_m = alpha + ibeta
std::complex<float> calcul_gm(float epsilon, float sigma, float omega) {
    float alpha = omega * sqrt(mu0 * epsilon / 2) * sqrt(sqrt(1 + (float)pow(sigma / (omega * epsilon), 2)) - 1.F);
    float beta = omega * sqrt(mu0 * epsilon / 2) * sqrt(sqrt(1 + (float)pow(sigma / (omega * epsilon), 2)) + 1.F);
    return alpha + 1if* beta;
}

std::complex<float> calcul_zm(emetteur Source, mur Mur) {
    float omega = Source.freq * 2 * M_PI;
    std::complex<float> epsilonTilde = Mur.permittivite - 1if* Mur.conductivite / omega;
    //std::complex<float> Zmcarre = mu0 / epsilonTilde;
    std::complex<float> Zm = sqrt(mu0 / epsilonTilde);
    return Zm;
}

//calcul les paramètres trigonométriques de transmission dans le mur
void calculTrigo(mur Mur, point Recepteur, point pSource, float& cosOi, float& cosOt, float& sinOi, float& sinOt, float& s) {
    std::vector<float> d{ Recepteur.x - pSource.x, Recepteur.y - pSource.y };  //vecteur entre TX et RX
    std::vector<float> n;                        //vecteurn normal au mûr
    if (Mur.p1.x == Mur.p2.x) {                  //s'il est vertical
        n = { Mur.p2.y - Mur.p1.y, 0 };
    }
    else if (Mur.p1.y == Mur.p2.y) {              //s'il est horizontal
        n = { 0, Mur.p1.x - Mur.p2.x };
    }
    else {                                       //s'il est oblique
        n = { Mur.p1.y - Mur.p2.y, Mur.p1.x - Mur.p2.x };
    }
    vecteurNorme(n); 
    vecteurNorme(d); 
    cosOi = abs(inner_product(d.begin(), d.end(), n.begin(), 0.0));
    sinOi = sqrt(1 - pow(cosOi, 2));
    sinOt = sqrt(epsilon0 / Mur.permittivite) * sinOi;
    cosOt = sqrt(1 - pow(sinOt, 2));
    s = Mur.epaisseur / cosOt;
}


//calcul le coefficient de transmission d'une polarisation perpendiculaire
std::complex<float> coeffTrans(mur Mur, point Recepteur, emetteur Source, point pSource,  std::complex<float> Zm, std::complex<float> gm) {
    float cosOi, cosOt, sinOi, sinOt, s;
    calculTrigo(Mur, Recepteur, pSource, cosOi, cosOt, sinOi, sinOt, s);
    
    float omega = 2 * M_PI * Source.freq;
    float beta = omega / c;

    std::complex<float> Rp = (Zm * cosOi - Z0 * cosOt) / (Zm * cosOi + Z0 * cosOt);
    std::complex<float> Tm = ((1.F - (std::complex<float>)pow(Rp, 2)) * exp(-gm * s)) / (1.0F - (std::complex<float>)pow(Rp, 2) * exp(-2.F * gm * s) * exp(beta * 1if * 2.F * s * sinOt * sinOi));
    return Tm;
}

//calcul le coefficient de réflexion d'une polarisation perpendiculaire
std::complex<float> coeffReflex(mur Mur, point Recepteur, emetteur Source, point pSource, std::complex<float> Zm, std::complex<float> gm) {
    float cosOi, cosOt, sinOi, sinOt, s;
    calculTrigo(Mur, Recepteur, pSource, cosOi, cosOt, sinOi, sinOt, s);

    float omega = 2 * M_PI * Source.freq; 
    float beta = omega / c; 

    std::complex<float> Rp = (Zm * cosOi - Z0 * cosOt) / (Zm * cosOi + Z0 * cosOt);
    std::cout << Z0 << std::endl;
    std::cout << cosOi << std::endl;
    std::cout << cosOt << std::endl;
    std::cout << Zm << std::endl;
    std::cout << Rp << std::endl; 
    std::complex<float> Rm = Rp - (1.F - Rp*Rp) *
        Rp*(exp(-2.F * gm * s) * exp( beta * 1if * 2.F * s * sinOt * sinOi)) /
        (1.F - Rp*Rp * exp(-2.F * gm * s) * exp(beta * 1if * 2.F * s * sinOt * sinOi));
    return Rm;
}

//calcule la puissance de l'émetteur dans la direction du récepteur en ca
float calculGTX(emetteur Source, point Recepteur) {
    //std::vector<float> dn{ Recepteur.x - Source.coordonnees.x, Recepteur.y - Source.coordonnees.y };
    // float GTX =  1/(8*pow(M_PI, 2))* pow((cos(M_PI/2 * cosO)/sinO), 2) * 32/3;
     //GTX = Source.PTX en produit scalire sur dn
    return  1.6977;
};



//////////////////////////////////////////

std::complex<float> ChampIncidentCarre(emetteur Source, point pSource, point Recepteur) {
    float GTX = calculGTX(Source, Recepteur);
    float d = distance(pSource, Recepteur);
    float omega = Source.freq * 2 * M_PI;
    float beta = omega / c;
    float he = Source.he;
    std::complex<float> expCarre = std::exp(-2*beta*d*1if);   // Je définis l'exponentielle dans la formule
    std::complex<float> EiCarre = 60.F * 1.64F *(float)pow(10, -3) * expCarre / (float)pow(d, 2);  // Je prend E au carré pour éviter de faire sqrt 
    return EiCarre;
}


// Puissance pour chemin direct
float PuissanceDirect(emetteur Source, point Recepteur) {
    float he = Source.he;
    float Ecarre = std::norm(ChampIncidentCarre(Source, Source.coordonnees, Recepteur));
    float Puissance = ( 1 / (8 * Source.resistance)) * Ecarre*pow(he, 2);  // !!!!!!!! ATTENTION !!!!!! FAUT VERIFEIR CA JFAIS UNE PTITE COUILLE avec les exponentielles
    return Puissance;
}

// Puissance par transmission
float PuissanceTransmission(emetteur Source, point Recepteur, mur Mur) {
    float permitivite, sigma, omega, he, beta, d;
    point pSource = Source.coordonnees;
    d = distance(pSource, Recepteur);
    he = Source.he;
    permitivite = Mur.permittivite;
    sigma = Mur.conductivite;
    omega = Source.freq * 2 * M_PI;
    beta = omega / c;
    std::complex<float> gm = calcul_gm(Mur.permittivite, sigma, omega);
    std::complex<float> Zm = calcul_zm(Source, Mur);
    std::complex<float> Tm = coeffTrans(Mur, Recepteur, Source, Source.coordonnees, Zm, gm);
    std::cout << gm<< Zm << Tm << std::endl;
    //std::complex<float> expCarre = exp(-2.F*beta*d*1if);
    float GTX = calculGTX(Source, Recepteur); 
    std::complex<float> EiCarre = Tm * sqrt(ChampIncidentCarre(Source, Source.coordonnees, Recepteur));
    float Pt = 1 / (8 * Source.resistance) * std::norm(EiCarre) * (float)pow(he, 2);
    std::cout << EiCarre << std::endl;
    return Pt;
}

//calcul du point de réflexion Pr sur le mûr 
void calculPtReflexion(point PSource, point Recepteur, mur Mur, point& PtReflexion, point& pt) {
    //std::vector<float> n = {Mur.p2.y - Mur.p1.y,Mur.p2.x - Mur.p1.x};
    //vecteurNorme(n); 
    
    std::vector<float> n = { 1, 0 }; 
    std::vector<float> vecteurMur = { Mur.p2.x - Mur.p1.x, Mur.p2.y - Mur.p1.y };
    vecteurNorme(vecteurMur); 

    std::vector<float> re = { PSource.x - Mur.p1.x, PSource.y - Mur.p1.y }; 
   // std::cout << "(" << re[0] << ", " << re[1] << ")" << std::endl;

    float ren = abs(inner_product(re.begin(), re.end(), n.begin(), 0.0)); 
    std::vector<float> ri = soustraireVecteur(re, multiplierVecteur(n, 2*ren));
   // std::cout << "(" << ri[0] << ", " << ri[1] << ")" << std::endl;
    pt = { ri[0], ri[1] }; 
    //std::cout << "(" << I.x << ", " << I.y << ")" << std::endl;

    std::vector<float> rx = { Recepteur.x - Mur.p1.x, Recepteur.y - Mur.p1.y };
    std::vector<float> d = soustraireVecteur(rx, ri);
    //std::cout << "(" << d[0] << ", " << d[1] << ")" << std::endl;

    float t = (d[1] * (ri[0] - Mur.p1.x) - d[0] * (ri[1] - Mur.p1.y)) / (vecteurMur[0] * d[1] - vecteurMur[1] * d[0]); 
    
    PtReflexion = { Mur.p1.x + t * vecteurMur[0], Mur.p1.y + t * vecteurMur[1] }; 
    //std::cout << "(" << PtReflexion.x << ", " << PtReflexion.y << ")" << std::endl;
}

// Puissance pour réflexion sheesha
float PuissanceReflexion(emetteur Source, point Recepteur, mur Mur1, mur Mur2) {
    float Pr, dr, Eicarre, GTX, PTX, beta, omega, he;
    std::complex<float> Zm1, gm1;
    point PSource = Source.coordonnees;
    point PtReflexion = point{}; 
    point I = point{}; 
    calculPtReflexion(PSource, Recepteur, Mur1, PtReflexion, I); 
    omega = Source.freq * 2 * M_PI;
    beta = omega / c;
    he = Source.he;
    dr = distance(PSource, PtReflexion) + distance(PtReflexion, Recepteur);
    GTX = calculGTX(Source, Recepteur);
    PTX = Source.PTX;
    Zm1 = calcul_zm(Source, Mur1);
    gm1 = calcul_gm(Mur1.permittivite, Mur1.conductivite, omega);
    std::complex<float> Rm = coeffReflex(Mur1, PtReflexion, Source, Source.coordonnees, Zm1, gm1);
    std::complex<float> Zm2 = calcul_zm(Source, Mur2);
    std::complex<float> gm2 = calcul_gm(Mur2.permittivite, Mur2.conductivite, omega);
    std::complex<float> Tm = coeffTrans(Mur2, PtReflexion, Source, Source.coordonnees, Zm2, gm2);
    std::cout << Tm << std::endl;
    std::complex<float> expCarre = std::exp(-2*beta*dr*1if);   
    std::complex<float> EiReflexCarre = Rm*Rm * Tm*Tm * ChampIncidentCarre(Source, I, Recepteur);
    std::cout << sqrt(EiReflexCarre)<< std::endl;
    Pr = 1 / (8 * Source.resistance) * std::norm(EiReflexCarre) * pow(he, 2);
    return Pr;
}



