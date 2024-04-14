#pragma once

#include <cmath>
#include "init_map_tp.h"
#include <vector>
#include <numeric>
#include <iostream>
#include <complex>
#include <cmath>
#include <math.h>

float distance(point p1, point p2);

float calculNorme(std::vector<float> unVecteur);

void vecteurNorme(std::vector<float>& unVecteur);

std::vector<float> additionerVecteur(std::vector<float> v1, std::vector<float> v2);

std::vector<float> soustraireVecteur(std::vector<float> v1, std::vector<float> v2);

std::vector<float> multiplierVecteur(std::vector<float> v, float scalaire);
float produitScalaire(std::vector<float> v1, std::vector<float> v2);

/////////////////////////////////////////////////

std::complex<float> calcul_gm(float epsilon, float sigma, float omega);
std::complex<float> calcul_zm(emetteur Source, mur Mur);

point calculPtReflexion(point PSource, point Recepteur, mur Mur);

point trouverSym(point pt, mur Mur);

void calculTrigoReflex(mur MurReflex, point Recepteur, point pSource, point sym,
    float& cosOi, float& cosOt, float& sinOi, float& sinOt, float& s);

void calculTrigoTrans(mur MurTrans, point Cible, point pSource,
    float& cosOi, float& cosOt, float& sinOi, float& sinOt, float& s);

std::complex<float> coeffTrans(mur Mur, point Recepteur, emetteur Source, point pSource,
    std::complex<float> Zm, std::complex<float> gm);

std::complex<float> coeffTransTotal(emetteur Source, point Cible, point pSource, float omega, std::vector<mur> MursTrans);

std::complex<float> coeffReflex(mur Mur, point Recepteur, emetteur Source, point pSource,
    std::complex<float> Zm, std::complex<float> gm, point sym);

float calculGTX(emetteur Source, point Recepteur);

bool sensTrigo(point p1, point p2, point p3);

bool segmentsIntersect(point p1, point p2, point p3, point p4);
