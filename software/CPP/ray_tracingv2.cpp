#include <iostream>
#include <complex.h>
#include "onde.h"
#include "init_map_tp.h"
#include "constantes.h"

float i = sqrt(-1); 

int main(void) {
    creerMap(mesMursVerticaux, mesMursHorizontaux, mesMursObliques);
    creerPoints(mesPoints);
    /*for (int i = 0; i < 81; i++) {
        for (int j = 0; j < 81; j++) {
            std::cout << mesPoints[i][j].x << std::endl;
            std::cout << mesPoints[i][j].y << std::endl;
            //std::cout << distance(mesPoints[i][j], point{ 0, 0 }) << std::endl;
        }
    }*/
    coefficientsMur(mesMursHorizontaux[1], monRecepteur, maSource, 2.45 * pow(10, 9));
    
    return 0;
}

