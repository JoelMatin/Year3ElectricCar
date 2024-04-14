#pragma once
#include "Field.h"

class Receiver
{
public: 
	const Point position; 
	const Antenna TX; 
	Segment direction = { TX.position, position }; 
	double he = -c / TX.frequency * 1 / Pi;
	Field E_receiver{ direction, TX }; 

	double power(); 

};

