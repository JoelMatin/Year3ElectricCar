#include "init_map_tp.h"
#include <iostream>

const int nbMursHor = 2;
const int nbMursVert = 1;
const int nbMursObl = 1;
const int nbPoints = 81;

mur   mesMursVerticaux[nbMursVert];
mur   mesMursHorizontaux[nbMursHor];
mur   mesMursObliques[nbMursObl];
point mesPoints[nbPoints][81];

int main(void) {
    creerMap(mesMursVerticaux, mesMursHorizontaux, mesMursObliques);
    creerPoints(mesPoints);
    for(int i = 0; i<81; i++){
        for(int j = 0; j<81; j++){
            std::cout << mesPoints[i][j].x << std:: endl;
            std::cout << mesPoints[i][j].y << std:: endl;
        }
    }
    return 0;
}
