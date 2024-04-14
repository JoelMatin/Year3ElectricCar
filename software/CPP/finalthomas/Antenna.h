#pragma once
#include "Point.h"
#include "vector"
#include "Calculations.h"

class Antenna {
public:
    Point position;
    double frequency;
    double R;
    double PTX;
    double number; 
    double omega = 2 * Pi * frequency;
    double beta = omega / c;

    double find_GTX3(Point RX);

    double find_GTX1_2(Point RX);
    float find_GTX(Point RX); 
}; 


