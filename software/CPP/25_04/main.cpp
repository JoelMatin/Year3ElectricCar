#include <iostream>
#include <complex.h>
#include "onde.h"
#include "init_map_tp.h"
#include "constantes.h"


const int nbMursHor = 2;
const int nbMursVert = 1;
const int nbMursObl = 1;


mur mesMursVerticaux[nbMursVert]; mur mesMursHorizontaux[nbMursHor]; mur mesMursObliques[nbMursObl];
point mesPoints[81][81];




mur monMurTransmis = { point{0,20}, point{80, 20}, (float)4.8*epsilon0, 0.018, 0.15 };


std::vector<mur> mesMursTransmissions = {monMurTransmis};


int main(void) {
    creerMap(mesMursVerticaux, mesMursHorizontaux, mesMursObliques);
    creerPoints(mesPoints);
    float P = PuissanceReflexion(maSource, monRecepteur, mesMursVerticaux[0], mesMursTransmissions);
    std::cout << "Puissance : "<< P<< std::endl;
    return 0;
}

