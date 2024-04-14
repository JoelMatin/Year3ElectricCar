//Created by David Moli on 19 / 04 / 2023.


#include "interface_graphique.h"
#include <thread>
#include <algorithm>
#include <string>


float largeurPointInterface = largeurPoint * facteurEchelle; 

std::vector<float> monDegrade = {}; 

point conversionLongueurPoint(point p) {          // Je convertis la longueur en mtre en longueur de l'interface graphique
    float x = (p.x + 30) * facteurEchelle;  // + 10 sinon c tro a gauche
    float y = (p.y + 25) * facteurEchelle;
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

sf::RectangleShape dessinerContourMap() {        // Je fais le contour en gris qui dlimite la map

    sf::RectangleShape contour(sf::Vector2f(largeur - 200, hauteur - 200));
    contour.setFillColor(sf::Color(230, 230, 230));
    contour.setPosition(100, 100);
    return contour;
};


sf::RectangleShape dessinerMur(mur Mur) {    // Sert  dfinir le rectangle  dessiner en fonction du mur que je souhaite plot

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

sf::RectangleShape* stockerMursDessin(std::array<mur, nbMursMap> arrayDeMurs) {   //Ici a me renvoit une liste de rectangle que je peux plot ensuite

    static sf::RectangleShape mursAPlot[nbMursMap];
    for (int i = 0; i < nbMursMap; i++) {
        mursAPlot[i] = dessinerMur(arrayDeMurs[i]);
    }
    return mursAPlot;
}
// Créer emetteur récepteur
sf::CircleShape dessinerCerle(point unPoint, bool Source) {
    point pointAPlot = conversionLongueurPoint(unPoint);
    sf::CircleShape cercleAPlot = sf::CircleShape(rayonCercle);
    cercleAPlot.setPosition(pointAPlot.x - 5, pointAPlot.y - 3);
    if (Source) {
        cercleAPlot.setFillColor(sf::Color(255, 0, 0));
    }
    else {
        cercleAPlot.setFillColor(sf::Color(0, 0, 255));
    }
    return cercleAPlot;
}
// Je cre mes chemins a partir de rectangles


// Ici permet de crer une onde partant d'un point  l'autre
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


    //sf::View view = window.getDefaultView();
    //view.setSize(largeur, -hauteur);
    //window.setView(view);
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

        window.draw(dessinerCerle(pSource, true));
        window.draw(dessinerCerle(Recepteur, false));

        // Dessine les ondes
        std::vector<sf::RectangleShape> listeOndes = dessinerOnde(pSource, Recepteur, MursReflex, MursDoubleReflex);

        for (int j = 0; j < listeOndes.size(); j++) {
            window.draw(listeOndes[j]);
        }

        // end the current frame
        window.display();
    }
}


sf::RectangleShape dessinerCarre(point unPoint, float puissance) {
    point pointAPlot = conversionLongueurPoint(unPoint);
    sf::RectangleShape rectAPlot(sf::Vector2f(largeurPointInterface, largeurPointInterface));
    rectAPlot.setPosition(pointAPlot.x-largeurPointInterface/2, pointAPlot.y-largeurPointInterface/2 );
    float puissancedBm = 10 * log10(puissance);
    if (puissancedBm > puissanceMax) {
        std::cout << puissancedBm << std::endl; 
    }
    float t = (float)(puissancedBm + 80) / 20.0f; 
    //std::cout << "t = " << t << std::endl; 
    if (t < 0) {
        t = 0;  
    }
    float r, g, b;
    if (t < 0.2) {
        // Mauve à Bleu
        r = 138 + t * (0 - 138) / 0.2;
        g = 43 + t * (47 - 43) / 0.2;
        b = 226 + t * (255 - 226) / 0.2;
    }
    else if (t < 0.4) {
        // Bleu à Vert
        r = 0 + (t - 0.2) * (0 - 0) / 0.2;
        g = 191 + (t - 0.2) * (255 - 191) / 0.2;
        b = 255 + (t - 0.2) * (0 - 255) / 0.2;
    }
    else if (t < 0.6) {
        // Vert à Jaune
        r = 0 + (t - 0.4) * (255 - 0) / 0.2;
        g = 255 + (t - 0.4) * (215 - 255) / 0.2;
        b = 0 + (t - 0.4) * (0 - 0) / 0.2;
    }
    else if (t < 0.8) {
        // Jaune à Rouge
        r = 255 + (t - 0.6) * (255 - 255) / 0.2;
        g = 215 + (t - 0.6) * (165 - 215) / 0.2;
        b = 0 + (t - 0.6) * (0 - 0) / 0.2;
    }
    else {
        // Rouge
        r = 255;
        g = 0;
        b = 0;
    }
    rectAPlot.setFillColor(sf::Color((int)r, (int)g, (int)b));

    return rectAPlot; 
}



std::vector<sf::RectangleShape> dessinerDegrade() {
    point p1 = conversionLongueurPoint({ 105, 0 });
    point p2 = conversionLongueurPoint({ 105, 70 });
    float epaisseur = 3 * facteurEchelle;
    float longueur = distance_interface(p1, p2);

    std::vector<sf::RectangleShape> vecteurClipRect{}; 
    float bas = 0; 
    float r = 0, g = 0, b = 0; 
    for (float t = 1; t >=0; t -=0.001) {
        if (t < 0.2) {
            // Mauve à Bleu
            r = 138 + t * (0 - 138) / 0.2;
            g = 43 + t * (47 - 43) / 0.2;
            b = 226 + t * (255 - 226) / 0.2;
        }
        else if (t < 0.4) {
            // Bleu à Vert
            r = 0 + (t - 0.2) * (0 - 0) / 0.2;
            g = 47 + (t - 0.2) * (255 - 47) / 0.2;
            b = 255 + (t - 0.2) * (0 - 255) / 0.2;
        }
        else if (t < 0.6) {
            // Vert à Jaune
            r = 0 + (t - 0.4) * (255 - 0) / 0.2;
            g = 255 + (t - 0.4) * (215 - 255) / 0.2;
            b = 0 + (t - 0.4) * (0 - 0) / 0.2;
        }
        else if (t < 0.8) {
            // Jaune à Rouge
            r = 255 + (t - 0.6) * (255 - 255) / 0.2;
            g = 215 + (t - 0.6) * (165 - 215) / 0.2;
            b = 0 + (t - 0.6) * (0 - 0) / 0.2;
        }
        else {
            // Rouge
            r = 255 + (t - 0.8) * (255 - 255) / 0.2;
            g = 165 + (t - 0.8) * (50-165) / 0.2;
            b = 0;
        }
        sf::RectangleShape clipRect(sf::Vector2f(longueur / 1000, epaisseur));
        clipRect.setRotation(90); 
        bas+=0.001;  
        clipRect.setPosition(p1.x, p1.y + bas*longueur);
        clipRect.setFillColor(sf::Color((int)r, (int)g, (int)b));
        vecteurClipRect.push_back(clipRect); 
    } 
    return vecteurClipRect; 
}



void dessinerPuissances(point pSource, std::vector<std::vector<float>> puissancesPoints1,
    std::vector<std::vector<float>> puissancesPoints2) {


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

        sf::Font font;
        if (!font.loadFromFile("arial.ttf"))
        {
            // Erreur lors du chargement de la police
            std::cout << "YA PROBLM" << std::endl;
        }

        sf::Text maxdB; 
        maxdB.setString("-60dBm");
        maxdB.setFont(font);
        maxdB.setCharacterSize(20);
        maxdB.setFillColor(sf::Color(0, 0, 0));
        point pointText = conversionLongueurPoint({ 105, (float)0});
        maxdB.setPosition(pointText.x, pointText.y);

        sf::Text mindB;
        mindB.setString("-80dBm");
        mindB.setFont(font);
        mindB.setCharacterSize(20);
        mindB.setFillColor(sf::Color(0, 0, 0));
        point pointText2 = conversionLongueurPoint({ 105, (float)67 });
        mindB.setPosition(pointText2.x, pointText2.y);
        
        window.draw(mindB); 
        window.draw(maxdB); 

        for (float i = 0; i < puissancesPoints1.size(); i++) {
            for (float j = 0; j < puissancesPoints1[1].size(); j++) {
                window.draw(dessinerCarre(mesPointsMap1[i][j], puissancesPoints1[i][j]));
            }
        }
        for (float m = 0; m < puissancesPoints2.size(); m++) {
            for (float n = 0; n < puissancesPoints2[1].size(); n++) {
                window.draw(dessinerCarre(mesPointsMap2[m][n], puissancesPoints2[m][n]));
            }
        }
        // Dessine les murs
        for (int i = 0; i < nbMursMap; i++) {
            window.draw(stockerMursDessin(mesMursMap)[i]);  // Je plot tous mes murs

        }
       
        window.draw(dessinerCerle(pSource, true));

        std::vector<sf::RectangleShape> vecteurClipRect = dessinerDegrade(); 
        for (int x = 0; x < vecteurClipRect.size(); x++) {
            window.draw(vecteurClipRect[x]); 
        }

        // end the current frame
        window.display();
    }
}