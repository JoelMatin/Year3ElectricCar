#include <iostream>
#include <complex.h>
#include "onde.h"
#include "init_map_tp.h"
#include "constantes.h"



int main(void) {
    creerMap();
    
    mur monMurTransmis = mesMursMap[1]; 
    std::vector<mur> mesMursTransmissions = { monMurTransmis }; 
    float P = puissanceReflexion(maSource, monRecepteur, mesMursMap[0], mesMursTransmissions);
    return 0;
}
