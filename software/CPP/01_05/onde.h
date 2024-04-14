#pragma once

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

std::complex<float> champTransTotal(const emetteur& Source, const point& Recepteur, const std::vector<mur>& MursTrans);

std::complex<float> champReflex(const emetteur& Source, const point& Recepteur, const mur& MurReflexion);

std::complex<float> champReflexTotal(const emetteur& Source, const point& Recepteur, const point& PtReflexion,
    const mur& MurReflex,const std::vector<mur>& MursTrans1, const std::vector<mur>& MursTrans2);

std::complex<float> champDoubleReflex(const emetteur& Source, const point& Recepteur, const mur& MurReflex1,
    const mur& MurReflex2, const point& I1, const point& I2, const point& PtReflexion2); 

std::complex<float> champDoubleReflexTotal(
    const emetteur& Source, const point& Recepteur,
    const point& I1, const point& I2, const point& PtReflexion1, const point& PtReflexion2,
    const std::vector<mur>& DeuxMursReflex,
    const std::vector<mur>& MursTrans1, const std::vector<mur>& MursTrans2, const std::vector<mur>& MursTrans3); 