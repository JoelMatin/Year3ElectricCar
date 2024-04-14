#include <array>
#include <iostream>
#include <cmath>
#include "init_map_tp.h"


void creerMur(float x1, float y1, float x2, float y2, float permittivite, float conductivite, float epaisseur, int pos, mur Murs[]) {
    Murs[pos].x1 = x1;
    Murs[pos].x2 = x2;
    Murs[pos].y1 = y1;
    Murs[pos].y2 = y2;
    Murs[pos].permittivite = permittivite;
    Murs[pos].conductivite = conductivite;
    Murs[pos].epaisseur = epaisseur;
}

void creerPoints(point Points[][81]) {
    for(int i = 0; i<81; i++){
        for(int j = 0; j<81; j++){
             float x = i;
             float y = j;
             Points[i][j] = point{x,y};
        }
    }
}

void creerMap(mur MursVert[], mur MursHor[], mur MursObl[]) {
    creerMur(0, 0, 0, 80, pow(4.25, -13), 0.018, 0.15, 0, MursVert);
    creerMur(0, 20, 80, 20, pow(4.25, -13), 0.018, 0.15, 0, MursHor);
    creerMur(0, 80, 80, 80, pow(4.25, -13), 0.018, 0.15, 1, MursHor);
    for (int i = 0; i < 2; i++) {
        std::cout << MursHor[i].x1 << "_" << MursHor[i].y1 << "_" << MursHor[i].x2 << "_" << MursHor[i].y2 << "_" << MursHor[i].permittivite << std::endl;
    }
}



