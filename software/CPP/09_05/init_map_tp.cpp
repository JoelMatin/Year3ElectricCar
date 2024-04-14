#include "init_map_tp.h"

std::vector<std::vector<point>> mesPointsMap1{};
std::vector<std::vector<point>> mesPointsMap2{}; 

void creerPoints1(std::vector<std::vector<point>>& mesPointsMap) {
    for (float i = 0; i < nbPointsX1; i++) {
        mesPointsMap.push_back(std::vector<point>{});
        for (float j = 0; j < nbPointsY1; j++) {
            mesPointsMap[i].push_back(point{ i * largeurPoint + largeurPoint/2 , j * largeurPoint + largeurPoint/2 });
        }
    }
}

void creerPoints2(std::vector<std::vector<point>>& mesPointsMap) {
    for (float i = 0; i < nbPointsX2; i++) {
        mesPointsMap.push_back(std::vector<point>{});
        for (float j = 0; j < nbPointsY2; j++) {
            mesPointsMap[i].push_back(point{ i * largeurPoint + largeurPoint/2 + 75, j * largeurPoint + largeurPoint/2 + 45 });
        }
    }
}

