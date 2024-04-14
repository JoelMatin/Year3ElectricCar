#include <cmath>
#include"init_map_tp.h"


struct emetteur {
    point coordonnees;
    float freq;
    float resistance;
    float lambda;
    float he;
    float PTX;
    std::vector<float> orientation;
    float delta;

};

struct onde{
    point ptDepart;
    point ptArrivee;
};

extern point monRecepteur;
extern emetteur maSource;

float distance(point p1, point p2);
    
std::complex<float> coefficientsDuMur(float freq, mur Mur);

float transmission(emetteur Source, point Recepteur, mur Mur);

std::complex<float> coefficientsMur(mur Mur, point Recepteur, emetteur Source, float freq);
