#include <iostream>
#include <complex.h>
#include "onde.h"
#include "init_map_tp.h"
#include "constantes.h"
#include "formule.h"
#include "raytracing.h"
#include "interface_graphique.h"


// Variables

point pSource = maSource.coordonnees;


int main(void) {
    
   // dessinerInterface(pSource, monRecepteur, trouverMursReflex(pSource, monRecepteur, mesMursMap), trouverMursDoubleReflex(pSource, monRecepteur, mesMursMap));

    
    
    float P = puissanceTotale(maSource, monRecepteur);
    std::cout << "Puissance: "<< P << std::endl << std::endl;


}
