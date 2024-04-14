#include <iostream>
#include <complex.h>
#include "onde.h"
#include "init_map_tp.h"
#include "constantes.h"
#include "formule.h"
#include "raytracing.h"


int main(void) {
    creerMap();
    /*
    std::vector<mur> empt = {}; 
    mur monMurTransmis = mesMursMap[1]; 
    std::vector<mur> mesMursTransmissions = { monMurTransmis }; 
    point PtReflexion = calculPtReflexion(maSource.coordonnees, monRecepteur, mesMursMap[0]); 
    float P = champReflexTotal(maSource, monRecepteur, PtReflexion, mesMursMap[0], mesMursTransmissions, empt);
    return 0;*/

    
    float P = puissanceTotale(maSource, monRecepteur); 
    std::cout << P << std::endl << std::endl;

}
