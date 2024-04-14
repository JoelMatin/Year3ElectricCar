
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
#include "interface_graphique.h"

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


    if (!(mesMursTrans.empty())) {
        ETransTotal = champTransTotal(Source, Recepteur, mesMursTrans);

    }
    if (!(mesMursReflex.empty())) {
        for (const mur& murReflex : mesMursReflex) {
            point PtReflexion = calculPtReflexion(pSource, Recepteur, murReflex);
            std::vector<mur> mursTrans1 = trouverMursTrans(pSource, PtReflexion, mesMursMap);
            std::vector<mur> mursTrans2 = trouverMursTrans(PtReflexion, Recepteur, mesMursMap);
      
            EReflexTotal += champReflexTotal(Source, Recepteur, PtReflexion, murReflex, mursTrans1, mursTrans2);

        }
    }
    if (!(mesMursDoubleReflex.empty())) {
        for (const std::vector<mur>& deuxMursReflex : mesMursDoubleReflex) {
            point I1 = trouverSym(pSource, deuxMursReflex[0]);
            point I2 = trouverSym(I1, deuxMursReflex[1]);
            
            point PtReflexion2 = calculPtReflexion(I1, Recepteur, deuxMursReflex[1]);
            point PtReflexion1 = calculPtReflexion(pSource, PtReflexion2, deuxMursReflex[0]);

          
            std::vector<mur> mursTrans1 = trouverMursTrans(pSource, PtReflexion1, mesMursMap);
            std::vector<mur> mursTrans2 = trouverMursTrans(PtReflexion1, PtReflexion2, mesMursMap);
            std::vector<mur> mursTrans3 = trouverMursTrans(PtReflexion2, Recepteur, mesMursMap);
           
            EDoubleReflexTotal += champDoubleReflexTotal(Source, Recepteur, I1, I2, PtReflexion1, PtReflexion2, deuxMursReflex,
                mursTrans1, mursTrans2, mursTrans3);
        }
    }

  /*  std::cout << "Etransmission:" << ETransTotal << std::endl;
    std::cout << "Ereflexion:"    << EReflexTotal << std::endl;
    std::cout << "Edoublereflex:" << EDoubleReflexTotal << std::endl;*/
    
    dessinerInterface(maSource.coordonnees, monRecepteur, mesMursReflex, mesMursDoubleReflex);

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
