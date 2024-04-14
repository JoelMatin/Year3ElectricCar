#pragma once

#include <array>
#include <iostream>
#include <cmath>
#include <vector>
#include "constantes.h"

const float largeurPoint = 5; 
const int nbPointsY1 = 45/largeurPoint;
const int nbPointsX1 = 100/largeurPoint;
const int nbPointsY2 = 25/largeurPoint;
const int nbPointsX2 = 25/largeurPoint;
const int nbMursMap = 25;

const float puissanceMax = -30;
const float puissanceMin = -80; 


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
    float he = -c / freq * 1 / M_PI;
};

struct mur {
    point p1;                   // p1.x <= p2.x et p1.y <= p2.y
    point p2;
    float permittivite;
    float conductivite;
    float epaisseur;
};

const emetteur maSourceTX2{ point{-10, 0.5}, 26 * (float)pow(10, 9), 73, 3162.277 , std::vector<float>(0, 0), 0 };
const emetteur maSourceTX3{ point{-10, 0.5}, 26 * (float)pow(10, 9), 73, 3162.277 , std::vector<float>(0, 0), 0 }; 

//const emetteur  maSource{ point{32, 10},868.3F * (float)pow(10,6), 73.0, 0.1, std::vector<float>(0, 0) , 0 };
const point     monRecepteur{ 47, 65 };


/*const std::array<mur, nbMursMap> mesMursMap = {
    {
    { {0.0, 0.0}, {0.0, 80.0}, 4.8* epsilon0, 0.018, 0.15},
    { {0.0, 20.0}, {80, 20.0}, 4.8* epsilon0, 0.018, 0.15},
    { {0.0, 80.0}, {80.0, 80.0}, 4.8* epsilon0, 0.018, 0.15}
}
};*/


const std::array<mur, nbMursMap> mesMursMap = {
    {
        //mur vertical gauche de brique
        {{0.0, 0.0}, {0.0, 45.0}, 4.6*epsilon0, 0.02, 0.3 },

        // Je commence par les murs horizontaux à gauche (comprends les 2 longs murs rouge et bleu) (6)
        { {0.0, 0.0}, {100.0, 0.0}, 4.6 * epsilon0, 0.02, 0.3},  // *

        { {0.0, 9.0}, {15, 9.0}, 2.25 * epsilon0, 0.04, 0.1},  // *
        { {0.0, 18.0}, {15.0, 18.0}, 2.25 * epsilon0, 0.04, 0.1},  //
        { {0.0, 27.0}, {15.0, 27.0}, 2.25 * epsilon0, 0.04, 0.1},
        { {0.0, 36.0}, {15, 36.0}, 2.25 * epsilon0, 0.04, 0.1},

        { {0.0, 45.0}, {75.0, 45.0}, 5 * epsilon0, 0.014, 0.5},

        // Murs verticaux à gauche de longueur 8 (6)
        { {15.0, 0.0}, {15.0, 4.0}, 2.25 * epsilon0, 0.04, 0.1},
        { {15.0, 5.0}, {15, 13.0}, 2.25 * epsilon0, 0.04, 0.1},
        { {15.0, 14.0}, {15.0, 22.0}, 2.25 * epsilon0, 0.04, 0.1},
        { {15.0, 23.0}, {15.0, 31.0}, 2.24 * epsilon0, 0.04, 0.1},
        { {15.0, 32.0}, {15, 40.0},  2.25 * epsilon0, 0.04, 0.1},
        { {15.0, 41.0}, {15.0, 45.0}, 2.25 * epsilon0, 0.04, 0.1},
        
        // J'ai finit les murs de gauche go faire les 4 obliques du milieu
        { {35.0, 20.0}, {40.0, 15.0}, 2.25 * epsilon0, 0.04, 0.1},
        { {35.0, 30.0}, {40, 35.0}, 2.25 * epsilon0, 0.04, 0.1},
        { {50.0, 15.0}, {55.0, 20.0}, 2.25 * epsilon0, 0.04, 0.1},
        { {50.0, 35.0}, {55.0, 30.0}, 2.25 * epsilon0, 0.04, 0.1},

        //5 murs en rouge en haut a droite
        { {85.0, 0.0}, {85.0, 12.8398}, 4.6 * epsilon0, 0.02, 0.3},
        {{85.0, 14.8398}, {85, 14.8398 + 12.8398}, 4.6 * epsilon0, 0.02, 0.3},
        { {100.0, 0.0}, {100.0, 70.0}, 4.6 * epsilon0, 0.02, 0.3},
        { {85.0, 27.6795}, {91.5, 27.6795}, 4.6 * epsilon0, 0.02, 0.3},
        { {93.5, 27.6795}, {100, 27.6795}, 4.6 * epsilon0, 0.02, 0.3},

        //Mur oblique de longueur 20 en rouge
        { {75.0, 45.0}, {85.0, 27.6795}, 4.6 * epsilon0, 0.02, 0.3},

        //3 derniers murs en bas a droite
        { {75.0, 45.0}, {75.0, 70.0}, 5 * epsilon0, 0.014, 0.5},
        { {75.0, 70.0}, {100.0, 70.0}, 5 * epsilon0, 0.014, 0.5},
    }

};



extern std::vector<std::vector<point>> mesPointsMap1;
extern std::vector<std::vector<point>> mesPointsMap2;

void creerPoints1(std::vector<std::vector<point>>& mesPointsMap);
void creerPoints2(std::vector<std::vector<point>>& mesPointsMap); 