#pragma once
#include <vector>
#pragma once
#include "Point.h"
#include <cmath>
#include <complex>
#include <numeric>


class Vectorials {
public: 
    double v1; 
    double v2; 

    double scalar_product(Vectorials vector2); 

    double vector_norm(); 

    void add_vectors(Vectorials vector2); 

    void multiply_vector(double factor); 

};