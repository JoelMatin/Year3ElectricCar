#include "raytracing.h"
#include "interface_graphique.h"


//void creerPoints(crree)

int main(void) {
    //crees les points de la map
    creerPoints1(mesPointsMap1);
    creerPoints2(mesPointsMap2);

   
    std::vector<std::vector<float>> puissancesPoints1 = puissanceMap(mesPointsMap1, maSourceTX2);
    std::vector<std::vector<float>> puissancesPoints2 = puissanceMap(mesPointsMap2, maSourceTX2);   
    for (int y = 0; y < puissancesPoints1[0].size(); y++) {
        std::cout << puissancesPoints1[0][y] << std::endl;
    }
    for (int y = 0; y < puissancesPoints1[1].size(); y++) {
        std::cout << puissancesPoints1[1][y] << std::endl;
    }

    dessinerPuissances(maSourceTX2.coordonnees, puissancesPoints1, puissancesPoints2);
    //float P = puissanceTotale(maSource, monRecepteur);
    //std::cout << P << std::endl << std::endl;
    return 0; 
}

