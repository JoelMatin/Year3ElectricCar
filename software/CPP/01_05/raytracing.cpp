/*if (!((std::min(murTrans.p1.x, murTrans.p2.x) > std::max(Source.x, Cible.x))
	|| (std::max(murTrans.p1.x, murTrans.p2.x) < std::min(Source.x, Cible.x))
	|| (std::min(murTrans.p1.y, murTrans.p2.y) > std::max(Source.y, Cible.y))
	|| (std::max(murTrans.p1.y, murTrans.p2.y) < std::min(Source.y, Cible.y))))*/

#define DEBUG

#ifdef DEBUG
#include <complex>
#include <cmath>
#include <vector>
#include <numeric>
#include <iostream>
#include <math.h>

#include"init_map_tp.h"
#include "formule.h"
#include "onde.h"
#include "constantes.h"

bool verifReflex(point Source, point Cible, mur Mur) {
	point I = trouverSym(Source, Mur); 
	//std::cout << I.x << I.y << std::endl; 
	return segmentsIntersect(I, Cible, Mur.p1, Mur.p2); 
}


/// <summary>: vérifie si deux mûrs donne un chemin de double réflexion vers une cible 
/// <param name="Mur1">: mûr de première réflexion
/// <param name="Mur2">: mûr de deuxième réflexion
/// <param name="compteur">: int pour savoir si on est au 1er ou 2eme appel de la fonction
/// <returns>: un booléen qui détermine si la réflexion a lieu à chaque appel, si la 2ème n'a pas lieu alors = 0

bool verifReflexDouble(point Depart, point Cible, mur Mur1, mur Mur2) {
	bool boolean = false; 
	point I1 = trouverSym(Depart, Mur1);
	if (verifReflex(I1, Cible, Mur2)) {
		point symCible = trouverSym(Cible, Mur2); 
		if (verifReflex(Depart, symCible, Mur1)) {
			boolean = true;
		}
	}
	return boolean;
}

//renvoie le vecteur des obstacles entre deux points
std::vector<mur> trouverMursTrans(point pSource, point Cible, std::array<mur, nbMursMap> MursMap) {
	std::vector<mur> mesMursTrans{};
	for (const mur murTrans : MursMap) {
		if (segmentsIntersect(pSource, Cible, murTrans.p1, murTrans.p2)) {
			mesMursTrans.push_back(murTrans);
		}
	}
	return mesMursTrans;
}

//renvoie le vecteur des murs de reflexion entre source et recepteur
std::vector<mur> trouverMursReflex(point pSource, point Cible, std::array<mur, nbMursMap> MursMap) {
	std::vector<mur> mesMursReflex{}; 
	for (const mur murReflex : MursMap) { 
		if (verifReflex(pSource, Cible, murReflex)) {
			mesMursReflex.push_back(murReflex);
		}
	}
	return mesMursReflex; 
}

//renvoie vecteur de vecteurs de 2 murs de doubles reflexion entre source et recepteur 
std::vector<std::vector<mur>> trouverMursDoubleReflex(const point& pSource, const point& Cible, const std::array<mur, nbMursMap>& MursMap) {
	std::vector<std::vector<mur>> mesMursDoubleReflex{};
	for (const mur& Mur1 : MursMap) {
		for (const mur& Mur2 : MursMap) {
			if (&Mur1 != &Mur2) {
				if (verifReflexDouble(pSource, Cible, Mur1, Mur2)) {
				std::cout << "Mur1= " << "(" << Mur1.p1.x << ", " << Mur1.p1.y << ")" << " " << "(" << Mur1.p2.x << ", " << Mur1.p2.y << ")" << std::endl;
				std::cout << "Mur2= " << "(" << Mur2.p1.x << ", " << Mur2.p1.y << ")" << " " << "(" << Mur2.p2.x << ", " << Mur2.p2.y << ")" << std::endl;
				mesMursDoubleReflex.push_back(std::vector<mur>{Mur1, Mur2});						//vecteur de vecteur de longueur 2 avec 1er et 2eme murs de reflex
				}
			}
		}
	}
	return mesMursDoubleReflex;
}


std::complex<float> champTotal(const emetteur& Source, const point& Recepteur){
	point pSource = Source.coordonnees; 
	std::complex<float> ETransTotal = 0; 
	std::complex<float> EReflexTotal = 0; 
	std::complex<float> EDoubleReflexTotal = 0; 
	std::vector<mur> mesMursTrans = trouverMursTrans(pSource, Recepteur, mesMursMap); 
	std::vector<mur> mesMursReflex = trouverMursReflex(pSource, Recepteur, mesMursMap);
	std::vector<std::vector<mur>> mesMursDoubleReflex = trouverMursDoubleReflex(pSource, 
	Recepteur, mesMursMap);

	//std::cout << "taille murs double reflex:" << mesMursDoubleReflex[1].size() << std::endl;
	if (!(mesMursTrans.empty())) {
	    ETransTotal = champTransTotal(Source, Recepteur, mesMursTrans); 
	}
	if (!(mesMursReflex.empty())) {
		for (const mur& murReflex : mesMursReflex) {
			point PtReflexion = calculPtReflexion(pSource, Recepteur, murReflex); 
			std::vector<mur> mursTrans1 = trouverMursTrans(pSource, PtReflexion, mesMursMap); 
			std::vector<mur> mursTrans2 = trouverMursTrans(PtReflexion, Recepteur, mesMursMap);
			//std::cout << "mursTrans1 vide:" << mursTrans1.empty() << std::endl; 
			EReflexTotal += champReflexTotal(Source, Recepteur, PtReflexion, murReflex, mursTrans1, mursTrans2); 
		}
	}
	if (!(mesMursDoubleReflex.empty())) {
		for (const std::vector<mur>& deuxMursReflex : mesMursDoubleReflex) {
			point I1 = trouverSym(pSource, deuxMursReflex[0]);
			point I2 = trouverSym(I1, deuxMursReflex[1]);
			std::cout << "I1: " << I1.x << ", " << I1.y << std::endl;
			std::cout << "I2: " << I2.x << ", " << I2.y << std::endl;
			point PtReflexion2 = calculPtReflexion(I1, Recepteur, deuxMursReflex[1]);
			point PtReflexion1 = calculPtReflexion(pSource, PtReflexion2, deuxMursReflex[0]);
			
			std::cout << "ptreflex1 " << PtReflexion1.x << ", " << PtReflexion1.y << std::endl; 
			std::cout << "ptreflex2 " << PtReflexion2.x << ", " << PtReflexion2.y << std::endl;
			std::vector<mur> mursTrans1 = trouverMursTrans(pSource, PtReflexion1, mesMursMap);
			std::vector<mur> mursTrans2 = trouverMursTrans(PtReflexion1, PtReflexion2, mesMursMap);
			std::vector<mur> mursTrans3 = trouverMursTrans(PtReflexion2, Recepteur, mesMursMap);
			int i = 0;
			for (const mur& murTrans : mesMursTrans) {
				std::cout << "Mes murs trans " << i << mesMursTrans[i].p1.x << "," << mesMursTrans[i].p1.y << "," << mesMursTrans[i].p2.x << "," << mesMursTrans[i].p2.y << std::endl;
				i++;

			}
			EDoubleReflexTotal += champDoubleReflexTotal(Source, Recepteur, I1, I2, PtReflexion1, PtReflexion2, deuxMursReflex, 
				mursTrans1, mursTrans2, mursTrans3);
		}
	}
	
	std::cout << "Etransmission: " << ETransTotal << std::endl; 
	std::cout << "Ereflexion: " << EReflexTotal << std::endl;
	std::cout << "Edoublereflex: " << EDoubleReflexTotal << std::endl;
	return ETransTotal + EReflexTotal + EDoubleReflexTotal; 
}


float puissanceTotale(emetteur Source, point Recepteur) {
	float he = Source.he;
	std::complex<float> ETotal = champTotal(Source, Recepteur);
	std::cout << "Etotal: " << ETotal << std::endl;
	float PTotal = 1 / (8 * Source.resistance) * std::norm(ETotal) * (float)pow(he, 2);
	return PTotal; 
}

#endif

// Fonction qui permet de définir le chemin d'une onde
std::vector<point> trouverCheminDirect(point pSource, point Recepteur, std::array<mur, nbMursMap> MursTotaux) {
	return { pSource, Recepteur };
}

std::vector<point> trouverCheminReflex(point Source, point Recepteur, std::array<mur, nbMursMap> MursTotaux)
{
	std::vector<point> cheminReturn = { Source };
	bool isReflexion;
	// Je vais boucler sur tous les murs de la map et checker si y a reflexion
	for (const mur& murMap : MursTotaux) {
		isReflexion = verifReflex(Source, Recepteur, murMap);
		if (isReflexion) {
			// y a reflexion je rajoute un point sur le chemin de l'onde
			point ptReflex = calculPtReflexion(Source, Recepteur, murMap);
			cheminReturn.push_back(ptReflex);
		}
	}
	cheminReturn.push_back(Recepteur);
	return cheminReturn;
}

std::vector<point> trouverCheminDoubleReflex(point pSource, point Recepteur, std::array<mur, nbMursMap> MursTotaux) {
	std::vector<point> cheminReturn = { pSource };
	bool isDoubleReflexion;

	for (const mur& murMap : MursTotaux) {
		for (const mur& murMap2 : MursTotaux) {
			isDoubleReflexion = verifReflexDouble(pSource, Recepteur, murMap, murMap2);

			if (isDoubleReflexion) {
				point I1 = trouverSym(pSource, murMap);
				point PtReflexion2 = calculPtReflexion(I1, Recepteur, murMap2);
				point PtReflexion1 = calculPtReflexion(pSource, PtReflexion2, murMap);
				cheminReturn.push_back(PtReflexion1);
				cheminReturn.push_back(PtReflexion2);
			}
		}
	}
	cheminReturn.push_back(Recepteur);
	return cheminReturn;
}

