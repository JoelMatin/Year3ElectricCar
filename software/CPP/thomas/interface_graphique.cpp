//Created by David Moli on 19 / 04 / 2023.


#include "interface_graphique.h"
#include <thread>
#include <algorithm>
#include <string>


float largeurPointInterface = receiver_area * facteurEchelle;

std::vector<float> monDegrade = {};

Point conversionLongueurPoint(Point p) {          // Je convertis la longueur en mtre en longueur de l'interface graphique
    float x = (p.x + 30) * facteurEchelle;  // + 10 sinon c tro a gauche
    float y = (p.y + 25) * facteurEchelle;
    Point PointReturn = { x, y };
    return PointReturn;

};

float distance_interface(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

float trouverAngleWall(Wall unWall) {
    return (atan2(unWall.p2.y - unWall.p1.y, unWall.p2.x - unWall.p1.x) * 180) / Pi;   // Je calcule l'angle que forme le Wall avec l'axe x
}

float trouverAngleOnde(Point p1Onde, Point p2Onde) {
    return (atan2(p2Onde.y - p1Onde.y, p2Onde.x - p1Onde.x) * 180) / Pi;   // Je calcule l'angle que forme le Wall avec l'axe x
}

sf::RectangleShape dessinerContourMap() {        // Je fais le contour en gris qui dlimite la map
    sf::RectangleShape contour(sf::Vector2f(largeur - 200, hauteur - 200));
    contour.setFillColor(sf::Color(230, 230, 230));
    contour.setPosition(100, 100);
    return contour;
};


sf::RectangleShape dessinerWall(Wall Wall) {    // Sert  dfinir le rectangle  dessiner en fonction du Wall que je souhaite plot
    Point p1 = conversionLongueurPoint(Wall.p1);
    Point p2 = conversionLongueurPoint(Wall.p2);
    float epaisseur = Wall.thickness * facteurEchelle;
    float longueurWall = distance_interface(p1, p2);
    sf::RectangleShape WallADessiner(sf::Vector2f(longueurWall, epaisseur));
    WallADessiner.setPosition(p1.x, p1.y);
    WallADessiner.setFillColor(sf::Color(0, 0, 0));
    WallADessiner.setRotation(trouverAngleWall(Wall));
    return WallADessiner;
};

std::vector<sf::RectangleShape> stockerWallsDessin(std::vector<Wall> arrayDeWalls) {   //Ici a me renvoit une liste de rectangle que je peux plot ensuite
    std::vector<sf::RectangleShape> WallsAPlot;
    for (int i = 0; i < arrayDeWalls.size(); i++) {
        WallsAPlot.push_back(dessinerWall(arrayDeWalls[i]));
    }
    return WallsAPlot;
}

// Créer emetteur récepteur
sf::CircleShape dessinerCerle(Point unPoint, bool Source) {
    Point PointAPlot = conversionLongueurPoint(unPoint);
    sf::CircleShape cercleAPlot = sf::CircleShape(rayonCercle);
    cercleAPlot.setPosition(PointAPlot.x - 5, PointAPlot.y - 3);
    if (Source) {
        cercleAPlot.setFillColor(sf::Color(255, 0, 0));
    }
    else {
        cercleAPlot.setFillColor(sf::Color(0, 0, 255));
    }
    return cercleAPlot;
}



// Ici permet de crer une onde partant d'un Point  l'autre
sf::RectangleShape creerOnde(Point p1Onde, Point p2Onde, int typeOnde) {
    Point p1 = conversionLongueurPoint(p1Onde);
    Point p2 = conversionLongueurPoint(p2Onde);
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



std::vector<Point> trouverCheminReflex(Point Source, Point Recepteur, std::vector<Wall> WallsReflex)
{
    std::vector<Point> cheminReturn = {};
    for (int i = 0; i < WallsReflex.size(); i++) {
        Point PtReflexion{};
        Point I = WallsReflex[i].get_sym(Source); 
        Segment s1 = { I, Recepteur };
        s1.segments_intersect({ WallsReflex[i].p1, WallsReflex[i].p2}, PtReflexion);
        cheminReturn.push_back(PtReflexion);
    }
    return cheminReturn;
}

std::vector<Point> trouverCheminDoubleReflex(Point pSource, Point Recepteur, std::vector<Wall> WallsDoubleReflex) {
    std::vector<Point> cheminReturn = {};
    for (int i = 0; i < WallsDoubleReflex.size(); i+=2) {
        Wall& Wall1 = WallsDoubleReflex[i];
        Wall& Wall2 = WallsDoubleReflex[i+1];
        Point I1 = Wall1.get_sym(pSource);
        Point I2 = Wall2.get_sym(I1); 
        Point PtReflexion2{};
        Point PtReflexion1{}; 
        Segment s2 = { I2, Recepteur };
        s2.segments_intersect({ Wall2.p1, Wall2.p2 }, PtReflexion2);
        Segment s1 = { I1, PtReflexion2 };
        s1.segments_intersect({ Wall1.p1, Wall1.p2 }, PtReflexion1); 
        cheminReturn.push_back(PtReflexion1);
        cheminReturn.push_back(PtReflexion2);
    }
    return cheminReturn;
}


std::vector<sf::RectangleShape> dessinerOnde(Point pSource, Point Recepteur, std::vector<Wall> WallsReflex,
    std::vector<Wall> WallsDoubleReflex) {

    int nOnde = 1;
    std::vector<Point> CheminReflexSimple = trouverCheminReflex(pSource, Recepteur, WallsReflex);
    std::vector<Point> CheminReflexDouble = trouverCheminDoubleReflex(pSource, Recepteur, WallsDoubleReflex);
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



void dessinerInterface(Point pSource, Point Recepteur, std::vector<Wall> WallsReflex,
    std::vector<Wall> WallsDoubleReflex)
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
        // Dessine les Walls
        for (int i = 0; i < project_walls.size(); i++) {
            window.draw(stockerWallsDessin(project_walls)[i]);  // Je plot tous mes Walls
        }

        window.draw(dessinerCerle(pSource, true));
        window.draw(dessinerCerle(Recepteur, false));

        // Dessine les ondes
        std::vector<sf::RectangleShape> listeOndes = dessinerOnde(pSource, Recepteur, WallsReflex, WallsDoubleReflex);

        for (int j = 0; j < listeOndes.size(); j++) {
            window.draw(listeOndes[j]);
        }

        // end the current frame
        window.display();
    }
}


sf::RectangleShape dessinerCarre(Point unPoint, float puissance) {
    Point PointAPlot = conversionLongueurPoint(unPoint);
    sf::RectangleShape rectAPlot(sf::Vector2f(largeurPointInterface, largeurPointInterface));
    rectAPlot.setPosition(PointAPlot.x - largeurPointInterface / 2, PointAPlot.y - largeurPointInterface / 2);
    float puissancedBm = 10 * log10(puissance * pow(10, 3));
    if (puissancedBm > max_power) {
        std::cout << puissancedBm << std::endl;
    }
    float t = (float)(puissancedBm + 80) / 20.0f;
    float r, g, b;
    if (puissancedBm > max_power) {
        std::cout << unPoint.x << ", " << unPoint.y << ": " << puissancedBm << std::endl;
        r = 255;
        g = 255;
        b = 255;
    }
    if (t < 0) {
        r = 255;
        g = 255;
        b = 255;
    }
    else if (t < 0.2) {
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
    Point p1 = conversionLongueurPoint({ 105, 0 });
    Point p2 = conversionLongueurPoint({ 105, 70 });
    float epaisseur = 3 * facteurEchelle;
    float longueur = distance_interface(p1, p2);

    std::vector<sf::RectangleShape> vecteurClipRect{};
    float bas = 0;
    float r = 0, g = 0, b = 0;
    for (float t = 1; t >= 0; t -= 0.001) {
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
            g = 165 + (t - 0.8) * (50 - 165) / 0.2;
            b = 0;
        }
        sf::RectangleShape clipRect(sf::Vector2f(longueur / 1000, epaisseur));
        clipRect.setRotation(90);
        bas += 0.001;
        clipRect.setPosition(p1.x, p1.y + bas * longueur);
        clipRect.setFillColor(sf::Color((int)r, (int)g, (int)b));
        vecteurClipRect.push_back(clipRect);
    }
    return vecteurClipRect;
}



void dessinerPuissances(Point pSource, std::vector<double> puissancesPoints) {


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
        Point PointText = conversionLongueurPoint({ 105, (float)0 });
        maxdB.setPosition(PointText.x, PointText.y);

        sf::Text mindB;
        mindB.setString("-80dBm");
        mindB.setFont(font);
        mindB.setCharacterSize(20);
        mindB.setFillColor(sf::Color(0, 0, 0));
        Point PointText2 = conversionLongueurPoint({ 105, (float)67 });
        mindB.setPosition(PointText2.x, PointText2.y);

        window.draw(mindB);
        window.draw(maxdB);

        for (float i = 0; i < puissancesPoints.size(); i++) {
             window.draw(dessinerCarre(project_receivers[i].position, puissancesPoints[i]));
            }
        
        // Dessine les Walls
        for (int i = 0; i < project_walls.size(); i++) {
            window.draw(stockerWallsDessin(project_walls)[i]);  // Je plot tous mes Walls

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