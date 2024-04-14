
#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "init_map_tp.h"
#include "onde.h"
#include "raytracing.h"


const int largeur = 1500;  const int hauteur = 1000;  const int facteurEchelle = 8.5;  const int epaisseurOnde = 1; const int rayonCercle = 5;
extern std::vector<float> monDegrade; 
const point finPtXReel = { 105, -5 }; const point finPtYReel = { -5, 80 };
const point ptOrigineReel = { -15, -5 };

const sf::Color couleurMin(255, 255, 0);
const sf::Color couleurMax(255, 0, 0);


point conversionLongueurPoint(point p);
float distance_interface(point p1, point p2);
float trouverAngleMur(mur unMur);
float trouverAngleOnde(point p1Onde, point p2Onde);


sf::RectangleShape dessinerContourMap();
sf::RectangleShape dessinerMur(mur Mur);
sf::RectangleShape* stockerMursDessin(mur arrayDeMurs[nbMursMap]);

sf::CircleShape dessinerCerle(point unPoint, bool Source); 



//std::vector<point> trouverCheminDirect(point pSource, point Recepteur, std::vector<mur> MursTotaux);
std::vector<point> trouverCheminReflex(point Source, point Recepteur, std::vector<mur> MursReflex);
std::vector<point> trouverCheminDoubleReflex(point pSource, point Recepteur, std::vector<std::vector<mur>> MursDoubleReflex);

sf::RectangleShape creerOnde(point p1Onde, point p2Onde, int typeOnde);
std::vector<sf::RectangleShape> dessinerOnde(point pSource, point Recepteur, std::vector<mur> MursReflex,
    std::vector<std::vector<mur>> MursDoubleReflex);

sf::RectangleShape dessinerCarre(point unPoint, float puissance); 
std::vector<sf::RectangleShape> dessinerDegrade(); 
void dessinerInterface(point pSource, point Recepteur, std::vector<mur> MursReflex,
    std::vector<std::vector<mur>> MursDoubleReflex);
void dessinerPuissances(point pSource, std::vector<std::vector<float>> puissancesPoints1,
    std::vector<std::vector<float>> puissancesPoints2); 





