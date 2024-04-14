//  Created by David Moli on 19/04/2023.

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

// VARIABLES //
const int largeur = 2000; const int hauteur = 1500; const int facteurEchelle = 15;  // Facteur echelle pour dessiner en grand
const float epsilon0 = 8.854 * pow(10, -12);
const int nbMursProjet = 25;

// STRUCTURES //
struct point{
    float x;
    float y;
};

struct mur {
    point p1;                   // p1.x <= p2.x et p1.y <= p2.y
    point p2;
    float permittivite{};
    float conductivite{};
    float epaisseur{};
};


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

sf::RectangleShape* stockerMursDessin(mur arrayDeMurs[nbMursProjet]){   //Ici ça me renvoit une liste de rectangle que je peux plot ensuite
    
    static sf::RectangleShape mursAPlot[nbMursProjet];
    for (int i = 0; i<nbMursProjet; i++){
        mursAPlot[i] = dessinerMur(arrayDeMurs[i]);
    }
    return mursAPlot;
}


int main()
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
        //window.draw(dessinerMur(MesMurs[0]));
        //sf::RectangleShape mursAplot[] = stockerMursDessin(MesMurs);
        
        for(int i= 0; i<nbMursProjet; i++){
            window.draw(stockerMursDessin(MesMursProjet)[i]);  // Je plot tous mes murs
        }

        // end the current frame
        window.display();
    }

    return 0;
}
