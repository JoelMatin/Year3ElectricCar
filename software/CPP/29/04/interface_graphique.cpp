//  Created by David Moli on 19/04/2023.

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "init_map_tp.h"
#include "onde.h"
#include "raytracing.h"

// VARIABLES //
const int largeur = 2000; const int hauteur = 1500; const int facteurEchelle = 15; const int epaisseurOnde = 1; // Facteur echelle pour dessiner en grand

//const int nbMursMap = 25;




// OBJETS INSTANCIES POUR TEST //
/* ATTENTION !!!!!!!!
 Les coordonnées ici commencent en haut à gauche donc y = 0 => je suis tout en haut de l'écran !!!
 */

// Les murs du tp 8
mur MesMurs[] ={
    { {40.0, 0.0}, {40.0, 100.0}, 6*epsilon0, 0.01, 0.3},  // premier mur
    { {40.0, 20.0}, {100, 20.0}, 6*epsilon0, 0.01, 0.3},  // deuxième mur
    { {40.0, 80.0}, {100.0, 80.0}, 6*epsilon0, 0.01, 0.3},  // troisième mur
};

mur MesMursProjet[] ={
    // Je commence par les murs horizontaux à gauche (comprends les 2 longs murs rouge et bleu) (6)
    { {0.0, 0.0}, {100.0, 0.0}, 6*epsilon0, 0.01, 0.3},  // *
    { {0.0, 9.0}, {10, 9.0}, 6*epsilon0, 0.01, 0.3},  // *
    { {0.0, 18.0}, {10.0, 18.0}, 6*epsilon0, 0.01, 0.3},  //
    { {0.0, 27.0}, {10.0, 27.0}, 6*epsilon0, 0.01, 0.3},
    { {0.0, 36.0}, {10, 36.0}, 6*epsilon0, 0.01, 0.3},
    { {0.0, 45.0}, {75.0, 45.0}, 6*epsilon0, 0.01, 0.3},
    // Murs verticaux à gauche de longueur 8 (6)
    { {15.0, 0.0}, {15.0, 4.0}, 6*epsilon0, 0.01, 0.3},
    { {15.0, 5.0}, {15, 13.0}, 6*epsilon0, 0.01, 0.3},
    { {15.0, 14.0}, {15.0, 22.0}, 6*epsilon0, 0.01, 0.3},
    { {15.0, 23.0}, {15.0, 31.0}, 6*epsilon0, 0.01, 0.3},
    { {15.0, 32.0}, {15, 40.0}, 6*epsilon0, 0.01, 0.3},
    { {15.0, 41.0}, {15.0, 45.0}, 6*epsilon0, 0.01, 0.3},
    // J'ai finit les murs de gauche go faire les 4 obliques du milieu
    { {35.0, 20.0}, {40.0, 15.0}, 6*epsilon0, 0.01, 0.3},
    { {35.0, 30.0}, {40, 35.0}, 6*epsilon0, 0.01, 0.3},
    { {50.0, 15.0}, {55.0, 20.0}, 6*epsilon0, 0.01, 0.3},
    { {50.0, 35.0}, {55.0, 30.0}, 6*epsilon0, 0.01, 0.3},
    // 5 murs en rouge en haut a droite
    { {85.0, 0.0}, {85.0, 12.8398}, 6*epsilon0, 0.01, 0.3},
    { {85.0, 14.8398}, {85, 14.8398+12.8398}, 6*epsilon0, 0.01, 0.3},
    { {100.0, 0.0}, {100.0, 27.6795}, 6*epsilon0, 0.01, 0.3},
    { {85.0, 27.6795}, {91.5, 27.6795}, 6*epsilon0, 0.01, 0.3},
    { {93.5, 27.6795}, {100, 27.6795}, 6*epsilon0, 0.01, 0.3},
    //Mur oblique de longueur 20 en rouge
    { {75.0, 45.0}, {85.0, 27.6795}, 6*epsilon0, 0.01, 0.3},
    // 3 derniers murs en bas a droite
    { {75.0, 45.0}, {75.0, 70.0}, 6*epsilon0, 0.01, 0.3},
    { {75.0, 70.0}, {100.0, 70.0}, 6*epsilon0, 0.01, 0.3},
    { {100.0, 27.6795}, {100.0, 70.0}, 6*epsilon0, 0.01, 0.3},
    
};



// FONCTIONS //

point conversionLongueurPoint(point p){          // Je convertis la longueur en mètre en longueur de l'interface graphique
    float x = (p.x+10) * facteurEchelle;  // + 10 sinon c tro a gauche
    float y = (p.y +10) * facteurEchelle;
    point pointReturn = {x, y};
    return pointReturn;
    
};

float distance(point p1, point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

float trouverAngleMur(mur unMur){
    return (atan2(unMur.p2.y - unMur.p1.y, unMur.p2.x - unMur.p1.x)*180)/M_PI;   // Je calcule l'angle que forme le mur avec l'axe x
}

float trouverAngleOnde(point p1Onde, point p2Onde){
    return (atan2(p2Onde.y - p1Onde.y, p2Onde.x - p1Onde.x)*180)/M_PI;   // Je calcule l'angle que forme le mur avec l'axe x
}

sf::RectangleShape dessinerContourMap(){        // Je fais le contour en gris qui délimite la map
    
    sf::RectangleShape contour(sf::Vector2f(largeur-200, hauteur-200));
    contour.setFillColor(sf:: Color(230, 230, 230));
    contour.setPosition(100, 100);
    return contour;
};


sf::RectangleShape dessinerMur(mur Mur){    // Sert à définir le rectangle à dessiner en fonction du mur que je souhaite plot
    
    point p1 = conversionLongueurPoint(Mur.p1);
    point p2 = conversionLongueurPoint(Mur.p2);
    float epaisseur = Mur.epaisseur*facteurEchelle;
    float longueurMur = distance(p1, p2);
    sf::RectangleShape murADessiner(sf::Vector2f(longueurMur, epaisseur));
    murADessiner.setPosition(p1.x, p1.y);
    murADessiner.setFillColor(sf:: Color(0, 0, 0));
    murADessiner.setRotation(trouverAngleMur(Mur));
    return murADessiner;
};

sf::RectangleShape* stockerMursDessin(mur arrayDeMurs[nbMursMap]){   //Ici ça me renvoit une liste de rectangle que je peux plot ensuite
    
    static sf::RectangleShape mursAPlot[nbMursMap];
    for (int i = 0; i<nbMursMap; i++){
        mursAPlot[i] = dessinerMur(arrayDeMurs[i]);
    }
    return mursAPlot;
}

// Je crée mes chemins a partir de rectangles


// Ici permet de créer une onde partant d'un point à l'autre
sf::RectangleShape creerOnde(point p1Onde, point p2Onde, int typeOnde){
    point p1 = conversionLongueurPoint(p1Onde);
    point p2 = conversionLongueurPoint(p2Onde);
    float longueurOnde = distance(p1, p2);
    
    sf::RectangleShape ondeADessioner(sf::Vector2f(longueurOnde, epaisseurOnde));
    
    ondeADessioner.setPosition(p1.x, p1.y);
    ondeADessioner.setRotation(trouverAngleOnde(p1, p2));
    
    switch (typeOnde) {
            
        case 2:
            ondeADessioner.setFillColor(sf::Color(255, 0, 0));      // Onde rouge si Reflexion
            break;
            
        case 3:
            ondeADessioner.setFillColor(sf::Color(0, 255, 0));      // Onde verte si double Reflex
            break;
            
        default:
            ondeADessioner.setFillColor(sf::Color(0, 0, 255));      // Onde bleue si Trans
            break;
    }
    
    return ondeADessioner;
    
}


std::vector<sf::RectangleShape> dessinerOnde( point pSource, point Recepteur, std::array<mur, nbMursMap> mursProjet){
    int nOnde = 1;
    //std::vector<point> CheminDirect = {pSource, Recepteur};
    std::vector<point> CheminReflexSimple = trouverCheminReflex(pSource, Recepteur, mursProjet);
    std::vector<point> CheminReflexDouble = trouverCheminDoubleReflex(pSource, Recepteur, mursProjet);
    std::vector<sf::RectangleShape> mesOndes;
    
    sf::RectangleShape OndeDirect = creerOnde(pSource, Recepteur, 1);
    mesOndes.push_back(OndeDirect);
    
    if(CheminReflexSimple.size() > 2){
        nOnde +=2;
        sf::RectangleShape OndeReflex1 = creerOnde(pSource, CheminReflexSimple[1], 2);
        mesOndes.push_back(OndeReflex1);
        sf::RectangleShape OndeReflex2 = creerOnde(CheminReflexSimple[1], Recepteur, 2);
        mesOndes.push_back(OndeReflex2);
    }
    
    if(CheminReflexDouble.size() > 2){
        nOnde +=3;
        sf::RectangleShape OndeDoubleR1 = creerOnde(pSource, CheminReflexDouble[1], 3);
        mesOndes.push_back(OndeDoubleR1);
        sf::RectangleShape OndeDoubleR2 = creerOnde(CheminReflexDouble[1], CheminReflexDouble[2], 3);
        mesOndes.push_back(OndeDoubleR2);
        sf::RectangleShape OndeDoubleR3 = creerOnde(CheminReflexDouble[2], Recepteur, 3);
        mesOndes.push_back(OndeDoubleR3);
    }
    

    return mesOndes;

}



void dessinerInterface()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(largeur, hauteur), "RayTransing");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with White color
        window.clear(sf::Color::White);

        // draw everything here...
        window.draw(dessinerContourMap());
        
        for(int i= 0; i<nbMursMap; i++){
            window.draw(stockerMursDessin(MesMurs)[i]);  // Je plot tous mes murs
        }

        // end the current frame
        window.display();
    }
}
