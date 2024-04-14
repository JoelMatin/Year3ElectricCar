#pragma once

#include <array>
#include <iostream>
#include <cmath>
#include <vector>
#include "constantes.h"

const int nbPointsY = 81;
const int nbPointsX = 81;

const int nbMursMap = 3;


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
    float permittivite;
    float conductivite;
    float epaisseur;
};


const emetteur  maSource{ point{32, 10},868.3F*(float)pow(10,6), 73.0, 0.1, std::vector<float>(0, 0) , 0};
const point     monRecepteur{ 47, 65 };

const std::array<mur, nbMursMap> mesMursMap = {
    {
    { {0.0, 0.0}, {0.0, 80.0}, 6*epsilon0, 0.01, 0.3},
    { {0.0, 20.0}, {80.0, 20.0}, 6*epsilon0, 0.01, 0.3},
    { {0.0, 80.0}, {80.0, 80.0}, 6*epsilon0, 0.01, 0.3}
}
    
};


const extern std::array<std::array<point, nbPointsX>, nbPointsY> mesPointsMap;




