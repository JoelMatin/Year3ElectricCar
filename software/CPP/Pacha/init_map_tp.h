#ifndef FUNCTIONS_H

#define FUNCTIONS_H
#include <array>
#include <iostream>
#include <cmath>
#include <vector>
#include "constantes.h"



const int nbPointsY = 81;
const int nbPointsX = 81;


struct point {
    float x{};
    float y{};
}; 

struct emetteur {
    point coordonnees;
    float freq;
    float resistance;
    float PTX;
    std::vector<float> orientation;
    float delta;
    float he = -c/freq *1/M_PI; 
};

struct mur {
    point p1;                   // p1.x <= p2.x et p1.y <= p2.y 
    point p2; 
    float permittivite{};
    float conductivite{};
    float epaisseur{};
};


void creerPoints(point Points[][81]);

void creerMur(point p1, point p2, float permittivite, float conductivite, float epaisseur, int pos, mur Murs[]);

void creerMap(mur MursVert[], mur MursHor[], mur MursObl[]);

float distance(point p1, point p2);

#endif
