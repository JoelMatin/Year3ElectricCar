#include "init_map_tp.h"


emetteur  maSource{ point{32, 10},868.3F*pow(10,6), 73.0, 0.1, std::vector<float>(0, 0) , 0};
point     monRecepteur{ 47, 65 };


std::array<mur, nbMursMap> mesMursMap{};
std::array<std::array<point, nbPointsX>, nbPointsY> mesPointsMap{};

std::array < mur, 5> TEST{}; 


//Crée la liste de points de la map;


//Crée la map avec les mûrs et les points; 
void creerMap() {
    mesMursMap[0] = { point{0, 0}, point{0, 80}, 4.8F * epsilon0, 0.018, 0.15};     //mur vertical

    mesMursMap[1] = { point {0, 20}, point{79, 20}, 4.8F * epsilon0, 0.018, 0.15 };    //murs horizontaux
    mesMursMap[2] = { point {0, 80}, point{79, 80}, 4.8F * epsilon0, 0.018, 0.15 };
}


