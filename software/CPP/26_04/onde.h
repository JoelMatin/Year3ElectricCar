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

void calculTrigoReflex(mur Mur, point Recepteur, point pSource, float& cosOi, float& cosOt, float& sinOi, float& sinOt, float& s);

void calculTrigoTrans(mur MurTrans, point Recepteur, point pSource, float& cosOi, float& cosOt, float& sinOi, float& sinOt, float& s); 

std::complex<float> coeffTrans(mur Mur, point Recepteur, emetteur Source, point pSource, std::complex<float> Zm, std::complex<float> gm);

std::complex<float> coeffReflex(mur Mur, point Recepteur, emetteur Source, point pSource, std::complex<float> Zm, std::complex<float> gm);

float calculGTX(emetteur Source, point Recepteur);

std::complex<float> ChampIncidentCarre(emetteur Source, point pSource, point Recepteur);

std::complex<float> ChampReflexion(emetteur Source, point Recepteur, mur MurReflexion);

float PuissanceDirect(emetteur Source, point Recepteur);

float PuissanceTransmission(emetteur Source, point Recepteur, mur Mur);

void calculPtReflexion(point PSource, point Recepteur, mur Mur, point& PtReflexion, point& pt);

float PuissanceReflexion(emetteur Source, point Recepteur, mur MurReflexion, std::vector<mur>& listeMursTransmission);

float produitScalaire(std::vector<float> v1, std::vector<float> v2);
