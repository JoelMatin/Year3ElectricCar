#pragma once
#include <complex>
#include "Calculations.h"
#include "Antenna.h"
#include "Vectorials.h"


class Wall
{
public: 
    Point p1{}; 
    Point p2{}; 
    double epsilon{}; 
    double sigma{}; 
    double thickness{}; 
      
    std::complex<double> find_small_gamma(double epsilon, double omega, double sigma); 
    std::complex<double> find_zm(Antenna TX); 
    const Point& get_sym(Point TX_pos); 
    std::vector<double> snell_law(Point start, Point finish);

    double find_GTX3(Antenna TX, Point RX); 
    double find_GTX1_2(Antenna TX, Point RX); 
    
};

