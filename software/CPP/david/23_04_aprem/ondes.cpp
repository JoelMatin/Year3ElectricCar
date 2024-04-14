#include <iostream>
#include <complex>
#include <cmath>
#include <math.h>
#include "onde.h"
#include <vector>
#include <numeric>
#include "constantes.h"

using namespace std::complex_literals;

emetteur  maSource{ point{0, 20}, static_cast<float>(2.45 * pow(10,9))};
point     monRecepteur{70, 20};

float distance(point p1, point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

//calcul de gamma_m = alpha + ibeta
std::complex<float> calcul_gm(float epsilon, float sigma, float omega) {
    float alpha = omega * sqrt(mu0 * epsilon / 2) * sqrt(sqrt(1 + (float)pow(sigma / (omega * epsilon), 2))-1.F);
    float beta  = omega * sqrt(mu0 * epsilon / 2) * sqrt(sqrt(1 + (float)pow(sigma / (omega * epsilon), 2))+1.F);
    return alpha + 1if * beta;
}

std::complex<float> calcul_zm(emetteur Source, mur Mur){
    float omega = Source.freq*2* M_PI;
    std::complex<float> epsilonTilde = Mur.permittivite - 1if * Mur.conductivite / omega;
    std::complex<float> Zmcarre = mu0 / epsilonTilde;
    std::complex<float> Zm = sqrt(mu0/epsilonTilde);
    return Zm;
}


float calculNorme(std::vector<float> unVecteur){
    return sqrt(inner_product(unVecteur.begin(), unVecteur.end(), unVecteur.begin(), 0.0));
}

//calcul les paramtres trigonomtriques de transmission dans le mr
void calculTrigo(mur Mur, point Recepteur, emetteur Source, float& cosOi, float& cosOt, float& sinOi, float& sinOt, float& s) {
    std::vector<float> d{ Recepteur.x - Source.coordonnees.x, Recepteur.y - Source.coordonnees.y };  //vecteur entre TX et RX
    std::vector<float> n;
    if (Mur.p1.x == Mur.p2.x) {                  //s'il est vertical
       n = { Mur.p1.y - Mur.p2.y, 0 };
    }
    else if(Mur.p1.y == Mur.p2.y) {              //s'il est horizontal
       n = {0, Mur.p1.x - Mur.p2.x};
    }
    else {                                       //s'il est oblique
       n = {Mur.p1.y - Mur.p2.y, Mur.p1.x - Mur.p2.x};
    }
    cosOi = abs(inner_product(d.begin(), d.end(), n.begin(), 0.0))/(calculNorme(d)*calculNorme(n));
    sinOi = sqrt(1 - pow(cosOi, 2));
    sinOt = sqrt(epsilon0 / Mur.permittivite) * sinOi;
    cosOt = sqrt(1 - pow(sinOt, 2));
    s     =  Mur.epaisseur/ cosOt;
}

void calculCosSin(emetteur Source, point Recepteur, float& cosO, float& sinO){
    std::vector<float> v = Source.orientation;
    std::vector<float> d{ Recepteur.x - Source.coordonnees.x, Recepteur.y - Source.coordonnees.y };
    cosO = abs(inner_product(d.begin(), d.end(), v.begin(), 0.0))/(calculNorme(d)*calculNorme(v));
    sinO = sqrt(1 - pow(cosO, 2));
}

// Calcul la loi de snell je mets les addresses de mes variables en argument pour return 2 variables (costhetha et sintheta)
void calculSnell(mur Mur, float sinThetaI, float& sinThetaT, float& cosThetaT){
    float epsilon_r = Mur.permittivite/epsilon0;
    sinThetaT = 1/sqrt(epsilon_r) * sinThetaT;
    cosThetaT = sqrt(1-pow(sinThetaT, 2));
}


//calcul le coefficient de transmission d'une polarisation perpendiculaire
std::complex<float> coeffTrans(mur Mur, point Recepteur, emetteur Source, std::complex<float> Zm, std::complex<float> gm) {
    float cosOi, cosOt, sinOi, sinOt, s;
    calculTrigo(Mur, Recepteur, Source, cosOi, cosOt, sinOi, sinOt, s);

    std::complex<float> g0 = calcul_gm(epsilon0, 0, Source.freq * 2 * M_PI);
    float beta0 = g0.real();

    std::complex<float> Rp = (Zm * cosOi - Z0 * cosOt) / (Zm * cosOi + Z0 * cosOt);
    std::complex<float> Tm = ((1.F - (std::complex<float>)pow(Rp, 2)) * exp(-gm * s)) / ( 1.0F - (std::complex<float>)pow(Rp, 2) *  exp(-2.F * gm * s) * exp(beta0*1if * 2.F * s * sinOt * sinOi));
    return Tm;
}

//calcul le coefficient de rflexion d'une polarisation perpendiculaire
std::complex<float> coeffReflex(mur Mur, point Recepteur, emetteur Source, std::complex<float> Zm, std::complex<float> gm) {
    float cosOi, cosOt, sinOi, sinOt, s;
    calculTrigo(Mur, Recepteur, Source, cosOi, cosOt, sinOi, sinOt, s);

    std::complex<float> g0 = calcul_gm(epsilon0, 0, Source.freq * 2 * M_PI);
    float beta0 = reinterpret_cast<float(&)[2]>(g0)[1];
    std::complex<float> Rp = (Zm * cosOi - Z0 * cosOt) / (Zm * cosOi + Z0 * cosOt);
    std::complex<float> Rm = Rp - ( 1.F-(std::complex<float>)pow(Rp, 2) ) *
        (exp( -2.F*gm * s ) * beta0*1if*2.F*s*sinOt*sinOi) /
        (1.F - (std::complex<float>)pow(Rp, 2) * exp(2.F * gm * s) * exp(beta0*1if * 2.F * s * sinOt * sinOi));
    return Rm;
}

//calcule la puissance de l'émetteur dans la direction du récepteur en ca
float calculGTX(emetteur Source, point Recepteur) {
    std::vector<float> dn { Recepteur.x - Source.coordonnees.x, Recepteur.y - Source.coordonnees.y };
    float cosO;
    float sinO;
   // float GTX =  1/(8*pow(M_PI, 2))* pow((cos(M_PI/2 * cosO)/sinO), 2) * 32/3;
    //GTX = Source.PTX en produit scalire sur dn
    return 1.655;
};

//calcul la puissance de transmission
float Transmission(std::complex<float> Tm, std::complex<float> Rm, emetteur Source, point Recepteur) {
    float dn = distance(Source.coordonnees, Recepteur);
    float GTX = calculGTX(Source, Recepteur);
    std::complex<float> g0 = calcul_gm(epsilon0, 0, Source.freq * 2 * M_PI);
    float beta0 = g0.real();
    float PRX = 2.F * ((float)pow(Source.lambda, 2) / (8 * (float)pow(M_PI, 2) * Source.resistance)) * Source.PTX * GTX *
    (float)pow(abs(Tm*exp(-1if*beta0*dn/dn)), 2);
    return 23.0;
};

//calcul la puissance de transmission en un point
std::complex<float> transmission(mur Mur, point Recepteur, emetteur Source, float freq) {

    float omega = 2 * M_PI * freq;
    std::complex<float> epsilonTilde = Mur.permittivite - 1if * Mur.conductivite / omega;
    std::complex<float> Zmcarre = mu0 / epsilonTilde;
    std::complex<float> Zm = sqrt(mu0/epsilonTilde);
    std::complex<float> gm = calcul_gm(Mur.permittivite, Mur.conductivite, 2.F*M_PI*freq);
    
    std::complex<float> Tm = coeffTrans(Mur, Recepteur, Source, Zm, gm);
    std::complex<float> Rm = coeffReflex(Mur, Recepteur, Source, Zm, gm);

    std::cout << gm<<std::endl<<Zm;
    return Zm;
}




//      CODE    DAVID    23/04         //

// But de ces fonctions: Calculer la puissance pour chaque mode d'émission //

// On va d'abord calculer le champ incident au carré c est utile pour la transmission et les chemins direct

float ChampIncidentCarre(emetteur Source, point Recepteur){
    float GTX = calculGTX(Source, Recepteur);
    point pSource = Source.coordonnees;
    float d = distance(pSource, Recepteur);
    float omega = Source.freq * 2 * M_PI;
    float beta = omega/c;
    float he = Source.he;
    std::complex<float> expcarre = std::exp(std::complex<float>(0, -2*beta*d));   // Je définis l'exponentielle dans la formule
    float Ei_carre = 60 * GTX * Source.PTX * std::norm(expcarre)/pow(d, 2);  // Je prend E au carré pour éviter de faire sqrt
    return Ei_carre;
}

// Ici on va calculer les puissances //

// Puissance pour chemin direct
float PuissanceDirect(emetteur Source, point Recepteur){
    float he = Source.he;
    float Ecarre = ChampIncidentCarre(Source, Recepteur);
    float Puissance = 1/(8*Source.resistance)* Ecarre * pow(he, 2);  // !!!!!!!! ATTENTION !!!!!! FAUT VERIFEIR CA JFAIS UNE PTITE COUILLE avec les exponentielles
    return Puissance;
}

// Puissance par transmission
float PuissanceTransmission(emetteur Source, point Recepteur, mur unMur){
    float permitivite, sigma, omega, he, beta, d;
    point pSource = Source.coordonnees;
    d = distance(pSource, Recepteur);
    he = Source.he;
    permitivite = unMur.permittivite;
    sigma = unMur.conductivite;
    omega = Source.freq * 2 * M_PI;
    beta = omega/c;
    std::complex<float> gm = calcul_gm(unMur.permittivite, sigma, omega);
    std::complex<float> Zm = calcul_zm(Source, unMur);
    std::complex<float> Tm = coeffTrans(unMur, Recepteur, Source, Zm, gm);
    std::complex<float> expcarre = std::exp(std::complex<float>(0, -2*beta*d));
    float GTX = calculGTX(Source, Recepteur);
    float Ecarre = pow(std::norm(Tm),2) *  GTX * Source.PTX * std::norm(expcarre)/pow(d, 2);
    float Puissance = 1/(8*Source.resistance)* Ecarre * pow(he, 2);
    return Puissance;
}

// Puissance pour réflexion sheesh
float PuissanceReflexion(emetteur Source, point Recepteur, mur Mur, point ptReflexion){
    float PuissanceReflexion, dr, Ei_carre, GTX, PTX, exp_carre, beta, cosThetaI, sinThetaI, cosThetaT, sinThetaT, s, omega,he;
    std::complex<float> Zm, gm;
    point PSource = Source.coordonnees;
    omega = Source.freq*2*M_PI;
    beta = omega/c;
    he = Source.he;
    dr = distance(PSource, ptReflexion) + distance(ptReflexion, Recepteur);
    GTX = calculGTX(Source, Recepteur);
    PTX = Source.PTX;
    Zm = calcul_zm(Source, Mur);
    gm = calcul_gm(Mur.permittivite, Mur.conductivite, omega);
    calculTrigo(Mur, Recepteur, Source, cosThetaI, cosThetaT, sinThetaI, sinThetaT, s);
    std::complex<float> coeff_reflexion = coeffReflex(Mur, Recepteur, Source, Zm, gm);
    std::complex<float> expcarre = std::exp(std::complex<float>(0, -2*beta*dr));   // Je définis l'exponentielle dans la formule
    float Ei_reflex_carre = 60 * GTX * Source.PTX * std::norm(expcarre)/pow(dr, 2);
    PuissanceReflexion = 1/(8*Source.resistance)* Ei_reflex_carre * pow(he, 2);
    return PuissanceReflexion;
}


