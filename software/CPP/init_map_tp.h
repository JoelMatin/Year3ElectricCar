#ifndef FUNCTIONS_H

#define FUNCTIONS_H


const int nbMursHor = 2;
const int nbMursVert = 1;
const int nbMursObl = 1;
const int nbPointsX = 81;
const int nbPointsY = 81;

struct point {
    float x{};
    float y{};
}; 


struct mur {
    point p1;                   // p1.x <= p2.x et p1.y <= p2.y 
    point p2; 
    float permittivite{};
    float conductivite{};
    float epaisseur{};
};



mur mesMursVerticaux[]; mur mesMursHorizontaux[]; mur mesMursObliques[]; point mesPoints[][nbPointsY];


void creerPoints(point Points[][nbPointsY]);

void creerMur(point p1, point p2, float permittivite, float conductivite, float epaisseur, int pos, mur Murs[]);

void creerMap(mur MursVert[], mur MursHor[], mur MursObl[]);

float distance(point p1, point p2);

#endif
