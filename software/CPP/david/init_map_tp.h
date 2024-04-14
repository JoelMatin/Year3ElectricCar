#ifndef FUNCTIONS_H

#define FUNCTIONS_H

struct emetteur {
    float x{};
    float y{};
};

struct point {
    float x{};
    float y{};
};

struct mur{
    float x1{};
    float x2{};
    float y1{};
    float y2{};
    float permittivite{};
    float conductivite{};
    float epaisseur{};
};

void creerPoints(point Points[][80]);
void creerMur(float x1, float y1, float x2, float y2, float permittivite, float conductivite, float epaisseur, int pos, mur Murs[]);

void creerMap(mur MursVert[], mur MursHor[], mur MursObl[]);

#endif
