
#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "create_receivers.h"


const int largeur = 1500;  const int hauteur = 1000;  const int facteurEchelle = 8.5;  const int epaisseurOnde = 1; const int rayonCercle = 5;
extern std::vector<float> monDegrade;
const Point finPtXReel = { 105, -5 }; const Point finPtYReel = { -5, 80 };
const Point ptOrigineReel = { -15, -5 };

const sf::Color couleurMin(255, 255, 0);
const sf::Color couleurMax(255, 0, 0);


Point conversionLongueurPoint(Point p);
float distance_interface(Point p1, Point p2);
float trouverAngleWall(Wall unWall);
float trouverAngleOnde(Point p1Onde, Point p2Onde);


sf::RectangleShape dessinerContourMap();
sf::RectangleShape dessinerWall(Wall Wall);
std::vector<sf::RectangleShape> stockerWallsDessin(std::vector<Wall> arrayDeWall);

sf::CircleShape dessinerCerle(Point unPoint, bool Source);



//std::vector<Point> trouverCheminDirect(Point pSource, Point Recepteur, std::vector<Wall> WallsTotaux);
std::vector<Point> trouverCheminReflex(Point Source, Point Recepteur, std::vector<Wall> WallsReflex);
std::vector<Point> trouverCheminDoubleReflex(Point pSource, Point Recepteur, std::vector<std::vector<Wall>> WallsDoubleReflex);

sf::RectangleShape creerOnde(Point p1Onde, Point p2Onde, int typeOnde);
std::vector<sf::RectangleShape> dessinerOnde(Point pSource, Point Recepteur, std::vector<Wall> WallsReflex,
    std::vector<std::vector<Wall>> WallsDoubleReflex);

sf::RectangleShape dessinerCarre(Point unPoint, float puissance);
std::vector<sf::RectangleShape> dessinerDegrade();
void dessinerInterface(Point pSource, Point Recepteur, std::vector<Wall> WallsReflex,
    std::vector<Wall> WallsDoubleReflex);
void dessinerPuissances(Point pSource, std::vector<double> puissancesPoints);





#pragma once
