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


//calcul les paramètres trigonométriques de transmission dans le mûr  
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
    cosOi = abs(inner_product(d.begin(), d.end(), n.begin(), 0.0));
    sinOi = sqrt(1 - pow(cosOi, 2)); 
    sinOt = sqrt(epsilon0 / Mur.permittivite) * sinOi; 
    cosOt = sqrt(1 - pow(sinOt, 2)); 
    s     =  Mur.epaisseur/ cosOt; 
}


//calcul le coefficient de transmission d'une polarisation perpendiculaire
std::complex<float> coeffTrans(mur Mur, point Recepteur, emetteur Source, std::complex<float> Zm, std::complex<float> gm) {
    float cosOi, cosOt, sinOi, sinOt, s; 
    calculTrigo(Mur, Recepteur, Source, cosOi, cosOt, sinOi, sinOt, s);

    std::complex<float> g0 = calcul_gm(epsilon0, 0, Source.freq * 2 * M_PI); 
    float beta0 = g0.real();

    std::complex<float> Rp = (Zm * cosOi - Z0 * cosOt) / (Zm * cosOi + Z0 * cosOt); 
    std::complex<float> Tm = ((1.F - (std::complex<float>)pow(Rp, 2)) * exp(-gm * s)) /
        ( 1.0F - (std::complex<float>)pow(Rp, 2) *  exp(-2.F * gm * s) * exp(beta0*1if * 2.F * s * sinOt * sinOi));
    return Tm;
}

//calcul le coefficient de réflexion d'une polarisation perpendiculaire
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
    //GTX = Source.PTX en produit scalire sur dn 
}

//calcul la puissance de transmission 
float Transmission(std::complex<float> Tm, std::complex<float> Rm, emetteur Source, point Recepteur) {
    float dn = distance(Source.coordonnees, Recepteur); 
    float GTX = calculGTX();
    std::complex<float> g0 = calcul_gm(epsilon0, 0, Source.freq * 2 * M_PI);
    float beta0 = g0.real();
    float PRX = 2.F * ((float)pow(Source.lambda, 2) / (8 * (float)pow(M_PI, 2) * Source.resistance)) * Source.PTX * GTX * 
        (float)pow(abs(Tm*exp(-1if*beta0*dn/dn)), 2);
}

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




