
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
#include "interface_graphique.h"
#include <thread>


//fonction recursive: si appelee avec compteur =1: verifie s'il y a refelxion simple 
//si appelee avec compteur = 2: verifie s'il y a reflexion double en s'appelant recursivement
bool verifReflex(point pSource, point Cible, mur Mur1, mur Mur2, int compteur) {
    switch (compteur) {
    case 1: {
        point I = trouverSym(pSource, Mur1);
        return segmentsIntersect(I, Cible, Mur1.p1, Mur1.p2);
        break; 
        }
    case 2: {
        bool boolean = false;
        point I1 = trouverSym(pSource, Mur1);
        if (verifReflex(I1, Cible, Mur2, Mur2, compteur-1)) {
            point PtReflexion2 = calculPtReflexion(I1, Cible, Mur2);
            if (verifReflex(pSource, PtReflexion2, Mur1, Mur1, compteur-1)) {
                boolean = true;
            }
        }
        return boolean;
        }
    }
}


/*
bool verifReflex(point Source, point Cible, mur Mur) {
    point I = trouverSym(Source, Mur);
    return segmentsIntersect(I, Cible, Mur.p1, Mur.p2);
}


/// <summary>: vrifie si deux mrs donne un chemin de double rflexion vers une cible
/// <param name="Mur1">: mr de premire rflexion
/// <param name="Mur2">: mr de deuxime rflexion
/// <param name="compteur">: int pour savoir si on est au 1er ou 2eme appel de la fonction
/// <returns>: un boolen qui dtermine si la rflexion a lieu  chaque appel, si la 2me n'a pas lieu alors = 0

bool verifReflexDouble(point Depart, point Cible, mur Mur1, mur Mur2) {
    bool boolean = false;
    point I1 = trouverSym(Depart, Mur1);
    if (verifReflex(I1, Cible, Mur2)) {
        point PtReflexion2 = calculPtReflexion(I1, Cible, Mur2); 
        //point symCible = trouverSym(Cible, Mur2);
        if (verifReflex(Depart, PtReflexion2, Mur1)) {
            boolean = true;
        }
    }
    return boolean;
}
*/

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
    for (const mur& murReflex : MursMap) {
        if (verifReflex(pSource, Cible, murReflex, murReflex, 1)) {
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
                if (verifReflex(pSource, Cible, Mur1, Mur2, 2)) {
                    point I1 = trouverSym(pSource, Mur1);
                    point I2 = trouverSym(I1, Mur2);
                    point PtReflexion2 = calculPtReflexion(I1, Cible, Mur2);
                    point PtReflexion1 = calculPtReflexion(pSource, PtReflexion2, Mur1);
                    mesMursDoubleReflex.push_back(std::vector<mur>{Mur1, Mur2});                        //vecteur de vecteur de longueur 2 avec 1er et 2eme murs de reflex
                }
            }
        }
    }
    return mesMursDoubleReflex;
}


std::complex<float> champTotal(const emetteur& Source, const point& Recepteur) {
    point pSource = Source.coordonnees;
    std::complex<float> ETransTotal = 0;
    std::complex<float> EReflexTotal = 0;
    std::complex<float> EDoubleReflexTotal = 0;
    std::vector<mur> mesMursTrans = trouverMursTrans(pSource, Recepteur, mesMursMap);
    std::vector<mur> mesMursReflex = trouverMursReflex(pSource, Recepteur, mesMursMap);
    std::vector<std::vector<mur>> mesMursDoubleReflex = trouverMursDoubleReflex(pSource,
        Recepteur, mesMursMap);
    std::vector<std::thread> threads;

    //std::cout << "taille murs double reflex:" << mesMursDoubleReflex[1].size() << std::endl;
    if (!(mesMursTrans.empty())) {
        threads.push_back(std::thread([&]() {
            ETransTotal = champTransTotal(Source, Recepteur, mesMursTrans);
            //std::cout << Recepteur.x << ", " << Recepteur.y << ": " << norm(ETransTotal) << std::endl;
            })); 
    }

    if (!(mesMursReflex.empty())) {
        threads.push_back(std::thread([&]() {
            for (const mur& murReflex : mesMursReflex) {
                point PtReflexion = calculPtReflexion(pSource, Recepteur, murReflex);
                std::vector<mur> mursTrans1 = trouverMursTrans(pSource, PtReflexion, mesMursMap);
                std::vector<mur> mursTrans2 = trouverMursTrans(PtReflexion, Recepteur, mesMursMap);
                //std::cout << "mursTrans1 vide:" << mursTrans1.empty() << std::endl;
                EReflexTotal += champReflexTotal(Source, Recepteur, PtReflexion, murReflex, mursTrans1, mursTrans2);
            }
            })); 
    }
    if (!(mesMursDoubleReflex.empty())) {
        threads.push_back(std::thread([&]() {
            for (const std::vector<mur>& deuxMursReflex : mesMursDoubleReflex) {
                point I1 = trouverSym(pSource, deuxMursReflex[0]);
                point I2 = trouverSym(I1, deuxMursReflex[1]);
                //std::cout << "I1: " << I1.x << ", " << I1.y << std::endl;
                //std::cout << "I2: " << I2.x << ", " << I2.y << std::endl;
                point PtReflexion2 = calculPtReflexion(I1, Recepteur, deuxMursReflex[1]);
                point PtReflexion1 = calculPtReflexion(pSource, PtReflexion2, deuxMursReflex[0]);

                //std::cout << "ptreflex1 " << PtReflexion1.x << ", " << PtReflexion1.y << std::endl;
                ///std::cout << "ptreflex2 " << PtReflexion2.x << ", " << PtReflexion2.y << std::endl;
                std::vector<mur> mursTrans1 = trouverMursTrans(pSource, PtReflexion1, mesMursMap);
                std::vector<mur> mursTrans2 = trouverMursTrans(PtReflexion1, PtReflexion2, mesMursMap);
                std::vector<mur> mursTrans3 = trouverMursTrans(PtReflexion2, Recepteur, mesMursMap);
                int i = 0;
                for (const mur& murTrans : mesMursTrans) {
                    //std::cout << "Mes murs trans " << i << mesMursTrans[i].p1.x << "," << mesMursTrans[i].p1.y << "," << mesMursTrans[i].p2.x << "," << mesMursTrans[i].p2.y << std::endl;
                    i++;
                }
                EDoubleReflexTotal += champDoubleReflexTotal(Source, Recepteur, I1, I2, PtReflexion1, PtReflexion2, deuxMursReflex,
                    mursTrans1, mursTrans2, mursTrans3);
            }
            })); 
    }

    //threads.push_back(std::thread([&]() {
       //dessinerInterface(maSourceTX2.coordonnees, Recepteur, mesMursReflex, mesMursDoubleReflex); })); 

    for (auto& thread : threads) {
        thread.join();
    }

    //std::cout << "Etransmission:" << ETransTotal << std::endl;
    //std::cout << "Ereflexion:"    << EReflexTotal << std::endl;
    //std::cout << "Edoublereflex:" << EDoubleReflexTotal << std::endl;
   
    return ETransTotal + EReflexTotal + EDoubleReflexTotal;
}


//calcule la puissance totale en un point
float puissanceTotale(emetteur Source, point Recepteur) {
    float he = Source.he;
    std::complex<float> ETotal = champTotal(Source, Recepteur);
    //std::cout << "Etotal: " << ETotal << std::endl;
    float PTotal = 1 / (8 * Source.resistance) * std::norm(ETotal) * (float)pow(he, 2);
    //std::cout << Recepteur.x << Recepteur.y << ": " << PTotal << std::endl;
    return PTotal;
}


//calcule la puissance en chaque point de la map et retourne un vecteur avec une puissance par point
std::vector<std::vector<float>> puissanceMap(std::vector<std::vector<point>> mesPointsMap, emetteur Source) {
    std::vector<std::vector<float>> puissancesPoints = {}; 
    for (float k = 0; k < mesPointsMap.size(); k++) {
        puissancesPoints.push_back(std::vector<float>{});
        for (float l = 0; l < mesPointsMap[1].size(); l++) {
            float P = puissanceTotale(Source, mesPointsMap[k][l]); 
            puissancesPoints[k].push_back(P);
            
        }
    }
    return puissancesPoints; 
}


