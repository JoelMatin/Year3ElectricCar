//
//  raytracing.h
//  ProjetTelecom
//
//  Created by David Moli on 28/04/2023.
//

#ifndef raytracing_h
#define raytracing_h

#include "onde.h"
#include "constantes.h"
#include <complex>
#include <cmath>
#include"init_map_tp.h"
#include <vector>
#include <numeric>
#include <iostream>
#include <math.h>


//const int nbMursMap = 6;

bool sensTrigo(point p1, point p2, point p3);
bool segmentsIntersect(point p1, point p2, point p3, point p4);
point trouverSym(point pt, mur Mur);
/*std::vector<mur> trouverMursTrans(point Source, point Cible, mur(&MursVert)[nbMursVert], mur(&MursHor)[nbMursHor], mur(&MursObl)[nbMursObl]);*/


bool verifReflex(point Source, point Cible, mur Mur);
bool verifReflexDouble(point Depart, point Cible, mur Mur1, mur Mur2 , int compteur);
std::vector<mur> trouverMursReflex(point Source, point Cible, std::array<mur, nbMursMap>);
std::vector <mur> trouverMursDoubleReflex(point Source, point Cible, std::array<mur, nbMursMap>);
std::vector<point> trouverCheminDirect(point pSource, point Recepteur, mur(&MursTotaux)[nbMursMap]);
std::vector<point> trouverCheminReflex(point Source, point Recepteur, std::array<mur, nbMursMap>);
std::vector<point> trouverCheminDoubleReflex(point pSource, point Recepteur, std::array<mur, nbMursMap>);
std::complex<float> champTotal(emetteur Source, point Recepteur, mur MurTransmission);
float puissanceTotale(emetteur Source, point Recepteur, mur MurReflex, std::vector<mur>& MursTrans);


#endif /* raytracing_h */
