/*if (!((std::min(murTrans.p1.x, murTrans.p2.x) > std::max(Source.x, Cible.x))
	|| (std::max(murTrans.p1.x, murTrans.p2.x) < std::min(Source.x, Cible.x))
	|| (std::min(murTrans.p1.y, murTrans.p2.y) > std::max(Source.y, Cible.y))
	|| (std::max(murTrans.p1.y, murTrans.p2.y) < std::min(Source.y, Cible.y))))*/

#define DEBUG

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
int sensTrigo(point p1, point p2, point p3) {
	float penteP1P2 = (p2.y - p1.y) / (p2.x - p1.x);     //pente de la droite P1P2
	float penteP1P3 = (p3.y - p1.y) / (p3.x - p1.x); 
	return penteP1P3 > penteP1P2;						
}

//vérifie s'il y a intersection entre segments p1p2 et p3p4 
int segmentsIntersect(point p1, point p2, point p3, point p4) {
	return ((sensTrigo(p1, p2, p3) != sensTrigo(p1, p2, p4)) & (sensTrigo(p1, p3, p4) & sensTrigo(p2, p3, p4))); 
}


//trouve le symétrique d'un point par rapport à un mûr
point trouverSym(point pt, mur Mur) {
	std::vector<float> n = {Mur.p2.y - Mur.p1.y,Mur.p2.x - Mur.p1.x};
	vecteurNorme(n);					//norme le vecteur normale pour les calculs

	std::vector<float> vecteurMur = { Mur.p2.x - Mur.p1.x, Mur.p2.y - Mur.p1.y };
	vecteurNorme(vecteurMur);                 

	std::vector<float> re = { pt.x - Mur.p1.x, pt.y - Mur.p1.y };

	float ren = produitScalaire(re, n);       //produit scalaire entre re et n 
	std::vector<float> ri = soustraireVecteur(re, multiplierVecteur(n, 2 * ren));
	point I = { Mur.p1.x + ri[0], Mur.p1.y + ri[1] };

	return I; 
}


/////////////////////////////////////////////////////////////////////////////////////////////////


std::vector<mur> trouverMursTrans(point Source, point Cible, mur(&MursVert)[nbMursVert],
	mur(&MursHor)[nbMursHor], mur(&MursObl)[nbMursObl]) {
	std::vector<mur> mesMursTrans{}; 
	for (const mur murTrans : MursVert) {
		if (segmentsIntersect(Source, Cible, murTrans.p1, murTrans.p2)) {
			mesMursTrans.push_back(murTrans); 
		}
	}
	for (const mur murTrans : MursHor) {
		if (segmentsIntersect(Source, Cible, murTrans.p1, murTrans.p2)) {
			mesMursTrans.push_back(murTrans);
		}
	}
	for (const mur murTrans : MursObl) {
		if (segmentsIntersect(Source, Cible, murTrans.p1, murTrans.p2)) {
			mesMursTrans.push_back(murTrans);
		}
	}
	return mesMursTrans; 
}

int verifReflex(point Source, point Cible, mur Mur) {
	point I = trouverSym(Source, Mur); 
	return segmentsIntersect(I, Cible, Mur.p1, Mur.p2); 
}


/// <summary>: vérifie si deux mûrs donne un chemin de double réflexion vers une cible 
/// <param name="Mur1">: mûr de première réflexion
/// <param name="Mur2">: mûr de deuxième réflexion
/// <param name="compteur">: int pour savoir si on est au 1er ou 2eme appel de la fonction
/// <returns>: un booléen qui détermine si la réflexion a lieu à chaque appel, si la 2ème n'a pas lieu alors = 0

int verifReflexDouble(point Source, point Cible, mur Mur1, mur Mur2 , int compteur) {
	int boolean = 1; 
	switch (compteur) {
	case 1:
		point I1 = trouverSym(Source, Mur1); 
		boolean *= verifReflexDouble(I1, Cible, Mur1, Mur2, 2);   //récursivité avec I comme nouvelle source et compteur = 2
		break; 
	case 2: 
		point I2 = trouverSym(Source, Mur2);				//ici Source est le I calculer dans le premier appel
		boolean *= verifReflex(I2, Cible, Mur2);			//vérifie si le symétrique du symétrique est réfléchie par le deuxième mûr
		break;
	}
	return boolean;
}

std::vector<mur> trouverMursReflex(point Source, point Cible, mur(&MursVert)[nbMursVert],
	mur(&MursHor)[nbMursHor], mur(&MursObl)[nbMursObl]) {
	std::vector<mur> mesMursReflex{}; 
	for (const mur murReflex : MursVert) { 
		if (verifReflex(Source, Cible, murReflex)) {
			mesMursReflex.push_back(murReflex);
		}
	}
	for (const mur murReflex : MursHor) {
		if (verifReflex(Source, Cible, murReflex)) {
			mesMursReflex.push_back(murReflex);
		}
	}
	for (const mur murReflex : MursObl) {
		if (verifReflex(Source, Cible, murReflex)) {
			mesMursReflex.push_back(murReflex);
		}
	}
}


std::vector <mur> trouverMursDoubleReflex(point Source, point Cible, mur(&MursVert)[nbMursVert],
	mur(&MursHor)[nbMursHor], mur(&MursObl)[nbMursObl]) {
	std::vector<mur> mesMursDoubleReflex{};
	for (const mur Mur1 : MursVert) {
		for (const mur Mur2 : MursVert) {
			if (verifReflexDouble(Source, Cible, Mur1, Mur2, 1)) {
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