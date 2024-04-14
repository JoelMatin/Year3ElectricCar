#pragma once
#include "Segment.h"
#include <complex>
#include "Calculations.h"
#include "Antenna.h"
#include <vector>
#include "Wall.h"
#include "createMap.h"


class Field {
public: 
	Segment direction; 
	Antenna TX; 

	std::vector<Wall> reflection_walls{};
	std::vector<Wall> double_reflection_walls{};


	std::complex<double> reflexion_coefficient(Wall wall, Point start, Point finish); 
	std::complex<double> transmission_coefficient(Point start, Point finish, std::vector<Wall> transmission_walls, int index);

	bool is_reflection(Wall wall, Point& TX_prime, Point& Pr); 
	bool is_double_reflection(Wall wall1, Wall wall2, Point& TX_prime, Point& TX_prime2, Point& Pr1, Point& Pr2);
	std::vector<Wall> get_transmission_walls(Point start, Point finish); 
	std::vector<Wall> get_reflection_walls(std::vector<Point>& reflection_points, std::vector<Point>& sym_points); 
	std::vector<Wall> get_double_reflection_walls(std::vector<Point>& reflection_points, std::vector<Point>& sym_points); 

	std::complex<double> E_transmission(); 
	std::complex<double> E_reflection();  
	std::complex<double> E_double_reflection(); 
	//std::complex<double> Field::double_reflection_field(Wall wall_1, Wall wall_2); 
};