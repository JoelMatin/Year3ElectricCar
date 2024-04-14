#include "Wall.h"
#include <iostream>

//calcul de gamma_m = alpha + ibeta
std::complex<double> Wall::find_small_gamma(double epsilon, double omega, double sigma) {
    double alpha = omega * sqrt(mu0 * epsilon / 2) * sqrt(sqrt(1 + pow(sigma / (omega * epsilon), 2)) - 1.0);
    double beta = omega * sqrt(mu0 * epsilon / 2) * sqrt(sqrt(1 + pow(sigma / (omega * epsilon), 2)) + 1.0);
    return alpha + i() * beta;
}

std::complex<double> Wall::find_zm(Antenna TX) {
    std::complex<double> z_m = sqrt(mu0 / (epsilon - i() * sigma / TX.omega));
    return z_m;
}

const Point& Wall::get_sym(Point TX_pos) {
    Vectorials v_n = { -(p2.y - p1.y) , p2.x - p1.x };               //normal vector
    v_n = { v_n.v1/ v_n.vector_norm(), 
        v_n.v2/ v_n.vector_norm() };
    Vectorials v_u = { p2.x - p1.x, p2.y - p1.y };                  //parallel vector
    v_u = { v_u.v1 / v_u.vector_norm(),
        v_u.v2 / v_u.vector_norm() };

    Vectorials v_TX = { TX_pos.x - p1.x, TX_pos.y - p1.y };                       //vector from wall Point to TX   
    double l = v_TX.scalar_product(v_n);  
    v_n.multiply_vector(-2 * l);
    v_TX.add_vectors(v_n);                                                        //v_TX becomes vector between  and TX' 
   
    const  Point TX_prime{ p1.x + v_TX.v1, p1.y + v_TX.v2 };                            //returns TX'
    return TX_prime; 

}


//calcul les paramtres trigonomtriques de transmission dans le mur
std::vector<double> Wall::snell_law(Point start, Point finish) {
    Vectorials v_d{ finish.x - start.x, finish.y - start.y };               //vector between either TX and RX or TX_prime and RX or TX_prime and Pr2 or TX_prime2 and RX 
    Vectorials v_n{ -(p2.y - p1.y), p2.x - p1.x };   
    v_d = { v_d.v1 / v_d.vector_norm(),
        v_d.v2 / v_d.vector_norm() };    
    v_n = { v_n.v1 / v_n.vector_norm(),
        v_n.v2 / v_n.vector_norm() };

    double cos_theta_i = abs(v_d.scalar_product(v_n));
    double sin_theta_i = sqrt(1 - pow(cos_theta_i, 2));
    double sin_theta_t = sqrt(eps0 / epsilon) * sin_theta_i;
    double cos_theta_t = sqrt(1 - pow(sin_theta_t, 2));
    double s = thickness / cos_theta_t;
    return { cos_theta_i, sin_theta_i, sin_theta_t, cos_theta_t, s }; 
 }

    
double Wall::find_GTX3(Antenna TX, Point RX) {
    double delta = -0.1; 
    double phi_3dB = Pi / 6;
    Vectorials v_TX_RX = { RX.x - TX.position.x , RX.y - TX.position.y }; 
    v_TX_RX = { v_TX_RX.v1 / v_TX_RX.vector_norm(), v_TX_RX.v2 / v_TX_RX.vector_norm() }; 
    double phi = acos(v_TX_RX.v1); 
    double GTXdB = 21.5836 - 12 * pow((phi - delta) / phi_3dB, 2);
    double GTX3 = pow(10, GTXdB / 10);
    return GTX3;
}

double Wall::find_GTX1_2(Antenna TX, Point RX) {
    return 1.7;
}
