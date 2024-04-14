#include <cmath>
#include"init_map_tp.h"
#include <vector>
#include <numeric>
#include <iostream>
#include <complex>
#include <cmath>
#include <math.h>


struct onde {
    point ptDepart;
    point ptArrivee;
};

extern point monRecepteur;
extern emetteur maSource;

float distance(point p1, point p2);

std::complex<float> calcul_gm(float epsilon, float sigma, float omega); 

std::complex<float> calcul_zm(emetteur Source, mur Mur); 

float calculNorme(std::vector<float> unVecteur); 

void calculTrigo(mur Mur, point Recepteur, emetteur Source, float& cosOi, float& cosOt, float& sinOi, float& sinOt, float& s); 

std::complex<float> coeffTrans(mur Mur, point Recepteur, emetteur Source, std::complex<float> Zm, std::complex<float> gm); 

std::complex<float> coeffReflex(mur Mur, point Recepteur, emetteur Source, std::complex<float> Zm, std::complex<float> gm);

float calculGTX(emetteur Source, point Recepteur); 

std::complex<float> ChampIncidentCarre(emetteur Source, point Recepteur); 

float PuissanceDirect(emetteur Source, point Recepteur); 

float PuissanceTransmission(emetteur Source, point Recepteur, mur Mur); 

point calculPtReflexion(point PSource, point Recepteur, mur Mur); 

float PuissanceReflexion(emetteur Source, point Recepteur, mur Mur1, mur Mur2); 