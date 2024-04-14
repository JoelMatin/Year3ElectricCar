#pragma once
#include <iostream>
#include "init_map_tp.h"
#include <complex>
#include <cmath>


struct emetteur {
    point coordonnees;
    float freq;
    float resistance; 
};

extern point monRecepteur; 
extern emetteur maSource; 

float distance(point p1, point p2); 
    
std::complex<float> coefficientsDuMur(float freq, mur Mur); 

float transmission(emetteur Source, point Recepteur, mur Mur); 

std::complex<float> coefficientsMur(mur Mur, point Recepteur, emetteur Source, float freq); 