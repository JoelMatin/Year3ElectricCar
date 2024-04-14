#include "Vectorials.h"

double Vectorials::scalar_product(Vectorials vector2) {
    return v1 * vector2.v1 + v2 * vector2.v2; 
}

double Vectorials::vector_norm() {
    return sqrt(scalar_product(*this));
}

 void Vectorials::add_vectors(Vectorials vector2) {
    v1 += vector2.v1; 
    v2 += vector2.v2; 
}

void Vectorials::multiply_vector(double factor) {
    v1 *= factor; 
    v2 *= factor; 
}


