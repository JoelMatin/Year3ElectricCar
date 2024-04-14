//  Created by David Moli on 19/04/2023.

#include "interface_graphique.h"
#include <thread>
point conversionLongueurPoint(point p) {          // Je convertis la longueur en mètre en longueur de l'interface graphique
    float x = (p.x + 30) * facteurEchelle;  // + 10 sinon c tro a gauche
    float y = (p.y + 10) * facteurEchelle;
    point pointReturn = { x, y };
    return pointReturn;

};

float distance_interface(point p1, point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

float trouverAngleMur(mur unMur) {
    return (atan2(unMur.p2.y - unMur.p1.y, unMur.p2.x - unMur.p1.x) * 180) / M_PI;   // Je calcule l'angle que forme le mur avec l'axe x
}

float trouverAngleOnde(point p1Onde, point p2Onde) {
    return (atan2(p2Onde.y - p1Onde.y, p2Onde.x - p1Onde.x) * 180) / M_PI;   // Je calcule l'angle que forme le mur avec l'axe x
}

sf::RectangleShape dessinerContourMap() {        // Je fais le contour en gris qui délimite la map

    sf::RectangleShape contour(sf::Vector2f(largeur - 200, hauteur - 200));
    contour.setFillColor(sf::Color(230, 230, 230));
    contour.setPosition(100, 100);
    return contour;
};


sf::RectangleShape dessinerMur(mur Mur) {    // Sert à définir le rectangle à dessiner en fonction du mur que je souhaite plot

    point p1 = conversionLongueurPoint(Mur.p1);
    point p2 = conversionLongueurPoint(Mur.p2);
    float epaisseur = Mur.epaisseur * facteurEchelle;
    float longueurMur = distance_interface(p1, p2);
    sf::RectangleShape murADessiner(sf::Vector2f(longueurMur, epaisseur));
    murADessiner.setPosition(p1.x, p1.y);
    murADessiner.setFillColor(sf::Color(0, 0, 0));
    murADessiner.setRotation(trouverAngleMur(Mur));
    return murADessiner;
};

sf::RectangleShape* stockerMursDessin(std::array<mur, nbMursMap> arrayDeMurs) {   //Ici ça me renvoit une liste de rectangle que je peux plot ensuite

    static sf::RectangleShape mursAPlot[nbMursMap];
    for (int i = 0; i < nbMursMap; i++) {
        mursAPlot[i] = dessinerMur(arrayDeMurs[i]);
    }
    return mursAPlot;
}

// Je crée mes chemins a partir de rectangles


// Ici permet de créer une onde partant d'un point à l'autre
sf::RectangleShape creerOnde(point p1Onde, point p2Onde, int typeOnde) {
    point p1 = conversionLongueurPoint(p1Onde);
    point p2 = conversionLongueurPoint(p2Onde);
    float longueurOnde = distance_interface(p1, p2);

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
/*
std::vector<point> trouverCheminDirect(point pSource, point Recepteur, std::vector<mur> MursTotaux) {
    return { pSource, Recepteur };
}*/

std::vector<point> trouverCheminReflex(point Source, point Recepteur, std::vector<mur> MursReflex)
{
    std::vector<point> cheminReturn = {};
    for (int i = 0; i < MursReflex.size(); i++) {
        point PtReflexion = calculPtReflexion(Source, Recepteur, MursReflex[i]);
        cheminReturn.push_back(PtReflexion);
    }
    return cheminReturn;
}

std::vector<point> trouverCheminDoubleReflex(point pSource, point Recepteur, std::vector<std::vector<mur>> MursDoubleReflex) {
    std::vector<point> cheminReturn = {};

    for (int i = 0; i < MursDoubleReflex.size(); i++) {
        const mur& Mur1 = MursDoubleReflex[i][0];
        const mur& Mur2 = MursDoubleReflex[i][1];
        point I1 = trouverSym(pSource, Mur1);
        point PtReflexion2 = calculPtReflexion(I1, Recepteur, Mur2);
        point PtReflexion1 = calculPtReflexion(pSource, PtReflexion2, Mur1);
        cheminReturn.push_back(PtReflexion1);
        cheminReturn.push_back(PtReflexion2);
    }
    return cheminReturn;
}


std::vector<sf::RectangleShape> dessinerOnde(point pSource, point Recepteur, std::vector<mur> MursReflex,
    std::vector<std::vector<mur>> MursDoubleReflex) {
    int nOnde = 1;
    std::vector<point> CheminReflexSimple = trouverCheminReflex(pSource, Recepteur, MursReflex);
    std::vector<point> CheminReflexDouble = trouverCheminDoubleReflex(pSource, Recepteur, MursDoubleReflex);
    std::vector<sf::RectangleShape> mesOndes;

    sf::RectangleShape OndeDirect = creerOnde(pSource, Recepteur, 1);
    mesOndes.push_back(OndeDirect);

    std::vector<std::thread> threads;

    if (!(CheminReflexSimple.empty())) {
            for (int i = 0; i < CheminReflexSimple.size(); i++) {

                mesOndes.push_back(creerOnde(pSource, CheminReflexSimple[i], 2));
                mesOndes.push_back(creerOnde(CheminReflexSimple[i], Recepteur, 2));
            }
        }
    

    if (!(CheminReflexDouble.empty())) {
        
            for (int i = 0; i < CheminReflexDouble.size(); i++) {

                if (i == 0) {
                    mesOndes.push_back(creerOnde(pSource, CheminReflexDouble[i], 3));
                }
                else if (i % 2 == 0) {
                    mesOndes.push_back(creerOnde(pSource, CheminReflexDouble[i], 3));
                }
                else {
                    mesOndes.push_back(creerOnde(CheminReflexDouble[i - 1], CheminReflexDouble[i], 3));
                    mesOndes.push_back(creerOnde(CheminReflexDouble[i], Recepteur, 3));
                }
            }
        }


    return mesOndes;
}



void dessinerInterface(point pSource, point Recepteur, std::vector<mur> MursReflex,
    std::vector<std::vector<mur>> MursDoubleReflex)
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(largeur, hauteur), "RayTransing");
    sf::View view = window.getDefaultView();
    view.setSize(largeur, -hauteur);
    window.setView(view);
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
        // Dessine les murs
        for (int i = 0; i < nbMursMap; i++) {
            window.draw(stockerMursDessin(mesMursMap)[i]);  // Je plot tous mes murs

        }

        // Dessine les ondes
        std::vector<sf::RectangleShape> listeOndes = dessinerOnde(pSource, Recepteur,MursReflex, MursDoubleReflex);

        for (int j = 0; j < listeOndes.size(); j++) {
            window.draw(listeOndes[j]);
        }

        // end the current frame
        window.display();
    }
}

