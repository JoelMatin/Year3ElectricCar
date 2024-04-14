#include <cmath>
#include "init_map_tp.h"
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

std::complex<float> champIncident(emetteur Source, point Recepteur, float d); 

float puissanceDirect(emetteur Source, point Recepteur); 

std::complex<float> champTransmission(emetteur Source, point Recepteur, mur MurTransmission); 

float puissanceTransmission(emetteur Source, point Recepteur, mur MurTransmission); 

std::complex<float> champReflexion(emetteur Source, point Recepteur, mur MurReflexion); 

float puissanceReflexion(emetteur Source, point Recepteur, mur MurReflex, std::vector<mur> MursTrans1, std::vector<mur> MursTrans2);

std::complex<float> champDoubleReflexion(emetteur Source, point Recepteur, mur MurReflex1,
    mur MurReflex2, point& I1, point& I2, point& PtReflexion2);

float puissanceDoubleReflexion(emetteur Source, point Recepteur, std::vector<mur>& MursReflex,
    std::vector<mur> MursTrans1, std::vector<mur> MursTrans2, std::vector<mur> MursTrans3); 