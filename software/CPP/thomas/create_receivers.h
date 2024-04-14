#pragma once
#include "Receiver.h"


const double receiver_area = 5;
const int nb_receivers_y = 70 / receiver_area;
const int nb_receivers_x = 100 / receiver_area;

extern std::vector<Receiver> project_receivers;
extern std::vector<double> project_powers; 

void create_map(Antenna TX);
void calculate_powers(); 