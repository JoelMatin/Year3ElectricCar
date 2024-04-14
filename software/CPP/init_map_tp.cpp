#include <array>
#include <iostream>
#include <cmath>
#include "init_map_tp.h"
#include "constantes.h"


mur   mesMursVerticaux[nbMursVert];
mur   mesMursHorizontaux[nbMursHor];
mur   mesMursObliques[nbMursObl];
point mesPoints[nbPointsX][nbPointsY];
 
//Ajoute un mûr dans la liste de murs correspondante; 
void creerMur(mur Murs[], point p1, point p2, float permittivite, float conductivite, float epaisseur, int pos) {
    Murs[pos].p1 = p1;
    Murs[pos].p2 = p2;
    Murs[pos].permittivite = permittivite;
    Murs[pos].conductivite = conductivite;
    Murs[pos].epaisseur = epaisseur;
}

//Crée la liste de points de la map;
void creerPoints(point Points[nbPointsX][nbPointsY]) {
    for (int i = 0; i < nbPointsX; i++) {
        for (int j = 0; j < nbPointsY; j++) {
            float x = i;
            float y = j;
            Points[i][j] = point{ x,y };
        }
    }
}

//Crée la map avec les mûrs et les points; 
void creerMap(mur MursVert[], mur MursHor[], mur MursObl[]) {
    creerMur(MursVert, point{35, 0}, point{35, 80}, 6 *epsilon0, 0.01, 0.3, 0);
    creerMur(MursHor, point{ 0, 20 }, point{ 80, 20 }, 6*epsilon0, 0.01, 0.3, 0); 
    creerMur(MursHor, point{ 0, 80 }, point{ 80, 80 }, 6 * epsilon0, 0.01, 0.3, 1);
}


