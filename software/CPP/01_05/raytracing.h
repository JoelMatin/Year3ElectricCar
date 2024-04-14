#pragma once

#include <complex>
#include <cmath>
#include <vector>
#include <numeric>
#include <iostream>
#include <math.h>

#include"init_map_tp.h"
#include "formule.h"
#include "onde.h"
#include "constantes.h"

bool verifReflex(point Source, point Cible, mur Mur); 

bool verifReflexDouble(point Depart, point Cible, mur Mur1, mur Mur2); 

std::vector<mur> trouverMursTrans(point pSource, point Cible, std::array<mur, nbMursMap> MursMap); 

std::vector<mur> trouverMursReflex(point pSource, point Cible, std::array<mur, nbMursMap> MursMap); 

std::vector<std::vector<mur>> trouverMursDoubleReflex(point pSource, point Cible, std::array<mur, nbMursMap> MursMap); 


std::complex<float> champTotal(emetteur Source, point Recepteur); 

float puissanceTotale(emetteur Source, point Recepteur); 
