/*if (!((std::min(murTrans.p1.x, murTrans.p2.x) > std::max(Source.x, Cible.x))
	|| (std::max(murTrans.p1.x, murTrans.p2.x) < std::min(Source.x, Cible.x))
	|| (std::min(murTrans.p1.y, murTrans.p2.y) > std::max(Source.y, Cible.y))
	|| (std::max(murTrans.p1.y, murTrans.p2.y) < std::min(Source.y, Cible.y))))*/

//#define DEBUG

#ifdef DEBUG



#include "onde.h"
#include "constantes.h"
#include <complex>
#include <cmath>
#include"init_map_tp.h"
#include <vector>
#include <numeric>
#include <iostream>
#include <math.h>


//vérifie si p1 p2 p3 sont énuméré dans le sens trigonométrique
bool sensTrigo(point p1, point p2, point p3) {
	float penteP1P2 = (p2.y - p1.y) / (p2.x - p1.x);     //pente de la droite P1P2
	float penteP1P3 = (p3.y - p1.y) / (p3.x - p1.x); 
	return penteP1P3 > penteP1P2;						
}

//vérifie s'il y a intersection entre segments p1p2 et p3p4 
bool segmentsIntersect(point p1, point p2, point p3, point p4) {
	return ((sensTrigo(p1, p2, p3) != sensTrigo(p1, p2, p4)) && (sensTrigo(p1, p3, p4) && sensTrigo(p2, p3, p4))); 
}



/////////////////////////////////////////////////////////////////////////////////////////////////

bool verifReflex(point Source, point Cible, mur Mur) {
	point I = trouverSym(Source, Mur); 
	return segmentsIntersect(I, Cible, Mur.p1, Mur.p2); 
}


/// <summary>: vérifie si deux mûrs donne un chemin de double réflexion vers une cible 
/// <param name="Mur1">: mûr de première réflexion
/// <param name="Mur2">: mûr de deuxième réflexion
/// <param name="compteur">: int pour savoir si on est au 1er ou 2eme appel de la fonction
/// <returns>: un booléen qui détermine si la réflexion a lieu à chaque appel, si la 2ème n'a pas lieu alors = 0

bool verifReflexDouble(point Depart, point Cible, mur Mur1, mur Mur2 , int compteur) {
	bool boolean = true; 
	switch (compteur) {
	case 1:
		point I1 = trouverSym(Depart, Mur1); 
		point PtReflexion2 = calculPtReflexion(I1, Cible, Mur2); 
		boolean = verifReflexDouble(I1, Cible, Mur1, Mur2, 2) && verifReflex(Depart, PtReflexion2, Mur1);   //récursivité avec I comme nouvelle source et compteur = 2
		break; 
	case 2: 
		boolean = verifReflex(Depart, Cible, Mur2);			//ici Source est le I calculer dans le premier appel, vérifie si le symétrique est réfléchie par le deuxième mûr
		break;
	}
	return boolean;
}



std::vector<mur> trouverMursTrans(point pSource, point Cible, std::array<mur, nbMursMap> MursMap) {
	std::vector<mur> mesMursTrans{};
	for (const mur murTrans : MursMap) {
		if (segmentsIntersect(pSource, Cible, murTrans.p1, murTrans.p2)) {
			mesMursTrans.push_back(murTrans);
		}
	}
	return mesMursTrans;
}

std::vector<mur> trouverMursReflex(point pSource, point Cible, std::array<mur, nbMursMap> MursMap) {
	std::vector<mur> mesMursReflex{}; 
	for (const mur murReflex : MursMap) { 
		if (verifReflex(pSource, Cible, murReflex)) {
			mesMursReflex.push_back(murReflex);
		}
	}
}


std::vector <mur> trouverMursDoubleReflex(point pSource, point Cible, std::array<mur, nbMursMap> MursMap) {
	std::vector<mur> mesMursDoubleReflex{};
	for (const mur Mur1 : MursMap) {
		for (const mur Mur2 : MursMap) {
			if (verifReflexDouble(pSource, Cible, Mur1, Mur2, 1)) {
				//calcule puissance en double reflex
			}
		}
	}
}


std::complex<float> champTotal(emetteur Source, point Recepteur, mur MurTransmission) {
	
}


float puissanceTotale(emetteur Source, point Recepteur, mur MurReflex, std::vector<mur>& MursTrans) {
	///mettre vérification du parcours
}

#endif