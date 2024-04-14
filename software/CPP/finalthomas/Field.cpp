#include "Field.h"
#include "Calculations.h"
#include <algorithm>

#define DEBUG
#ifdef DEBUG



std::complex<double> Field::reflexion_coefficient(Wall wall, Point start, Point finish) {

    std::vector<double> parameters = wall.snell_law(start, finish);
    double cos_theta_i = parameters[0];
    double sin_theta_i = parameters[1];
    double sin_theta_t = parameters[2];
    double cos_theta_t = parameters[3];
    double s = parameters[4];

    std::complex<double> small_gamma = wall.find_small_gamma(wall.epsilon, TX.omega, wall.sigma);
    std::complex<double> z_m = wall.find_zm(TX);

    std::complex<double> gamma_p = (z_m * cos_theta_i - z0 * cos_theta_t) / (z_m * cos_theta_i + z0 * cos_theta_t);
    std::complex<double> gamma_m = gamma_p - (1.0 - pow(gamma_p, 2)) * gamma_p * (exp(-2.0 * small_gamma * s) *
        exp(i() * TX.beta * 2.0 * s * sin_theta_t * sin_theta_i)) /
        (1.0 - pow(gamma_p, 2) * exp(-2.0 * small_gamma * s) * exp(i() * TX.beta * 2.0 * s * sin_theta_t * sin_theta_i));
    return gamma_m;
}


std::complex<double> Field::transmission_coefficient(Point start, Point finish, std::vector<Wall> local_transmission_walls, int index) {
    if (index < 0) {
        return 1;
    }
    Wall current_wall = local_transmission_walls[index];
    std::complex<double> small_gamma = current_wall.find_small_gamma(current_wall.epsilon, TX.omega, current_wall.sigma);
    std::complex<double> z_m = current_wall.find_zm(TX);
    std::vector<double> parameters = current_wall.snell_law(start, finish);
    double cos_theta_i = parameters[0];
    double sin_theta_i = parameters[1];
    double sin_theta_t = parameters[2];
    double cos_theta_t = parameters[3];
    double s = parameters[4];
    std::complex<double> gamma_p = (z_m * cos_theta_i - z0 * cos_theta_t) / (z_m * cos_theta_i + z0 * cos_theta_t);
    std::complex<double> coeff = ((1.0 - pow(gamma_p, 2)) * exp(-small_gamma * s)) /
        (1.0 - pow(gamma_p, 2) * exp(-2.0 * small_gamma * s) * exp(i() * TX.beta * 2.0 * s * sin_theta_t * sin_theta_i));
    //appel récursif 
    std::complex<double> Tm = transmission_coefficient(start, finish, local_transmission_walls, index - 1);
    return coeff * Tm;
}



bool Field::is_reflection(Wall wall, Point& TX_prime, Point& Pr) {
    TX_prime = wall.get_sym(direction.p1); 
    //std::cout << TX_prime.x << ", " << TX_prime.y << std::endl; 
    Segment s = { TX_prime, direction.p2 }; 
    return s.segments_intersect({ wall.p1, wall.p2 }, Pr); 
}


bool Field::is_double_reflection(Wall wall1, Wall wall2, Point& TX_prime, Point& TX_prime2, Point& Pr1, Point& Pr2) {
    TX_prime = wall1.get_sym(direction.p1); 
    TX_prime2 = wall2.get_sym(TX_prime); 
    Segment s2 = { TX_prime2, direction.p2 }; 
    if (s2.segments_intersect({ wall2.p1, wall2.p2 }, Pr2)) {
        Segment s1 = { TX_prime, Pr2 }; 
        if (s1.segments_intersect({ wall1.p1, wall1.p2 }, Pr1)) {
            return true; 
        }
        else {
            return false; 
        }
    }
    else {
        return false; 
    }
}

std::vector<Wall> Field::get_transmission_walls(Point start, Point finish) {
    std::vector<Wall> transmission_walls; 
    for ( std::vector<Wall>::const_iterator it = project_walls.begin(); it != project_walls.end(); ++it) {
        Wall current_wall = *it; 
        Point Pt = {};                                                          
        Segment s = { start, finish }; 
        if (s.segments_intersect({current_wall.p1, current_wall.p2}, Pt)) {
            transmission_walls.push_back(current_wall); 
        }
    }
    return transmission_walls; 
}

std::vector<Wall> Field::get_reflection_walls(std::vector<Point>& reflection_points, std::vector<Point>& sym_points) {
    for (std::vector<Wall>::const_iterator it = project_walls.begin(); it != project_walls.end(); ++it) {
        Wall current_wall = *it;
        Point Pr = {};                                                          
        Point TX_prime{}; 
        if (is_reflection(current_wall, TX_prime, Pr)) {
            reflection_walls.push_back(current_wall);
            reflection_points.push_back(Pr);
            sym_points.push_back(TX_prime); 
        }
    }
    return reflection_walls;
}

std::vector<Wall> Field::get_double_reflection_walls(std::vector<Point>& reflection_points, std::vector<Point>& sym_points) {
    for (int k = 0; k < project_walls.size(); ++k) {
        int index = 0;
        while (index < project_walls.size()) {
            if (index == k) {
                ++index;
                continue;
            }
            const Wall& wall1 = project_walls[k];
            const Wall& wall2 = project_walls[index];
            Point TX_prime{};
            Point TX_prime2{};
            Point Pr1{};
            Point Pr2{};
            if (is_double_reflection(wall1, wall2, TX_prime, TX_prime2, Pr1, Pr2)) {
                double_reflection_walls.push_back(wall1);
                double_reflection_walls.push_back(wall2);
                reflection_points.push_back(Pr1); 
                reflection_points.push_back(Pr2); 
                sym_points.push_back(TX_prime); 
                sym_points.push_back(TX_prime2); 
            }
            ++index;
        }
    }
    return double_reflection_walls; 
}


std::complex<double> Field::E_transmission() {
    std::vector<Wall> transmission_walls = get_transmission_walls(direction.p1, direction.p2); 
    double d = direction.p1.distance(direction.p2); 
    std::complex<double> Tm = transmission_coefficient(direction.p1, direction.p2, transmission_walls, 
        transmission_walls.size()-1); 
    double GTX = TX.find_GTX(direction.p2); 
    std::complex<double> field = Tm * sqrt(60.F * GTX * TX.PTX) * exp(-1.0 * i() * TX.beta * d) / d;
    return field; 
}

std::complex<double> Field::E_reflection() {
    std::complex<double> field = 0; 
    std::vector<Point> reflection_points{}; 
    std::vector<Point> sym_points{}; 
    get_reflection_walls(reflection_points, sym_points);      

    for (int k = 0; k < reflection_walls.size(); k ++) {
        Wall current_wall = reflection_walls[k]; 
       
        const double d = direction.p1.distance(reflection_points[k]) + reflection_points[k].distance(direction.p2); 
        std::vector<Wall> transmission_walls_1 = get_transmission_walls(direction.p1, reflection_points[k]);
        //std::cout << transmission_walls_1[0].p1.x << ", " << transmission_walls_1[0].p1.y << " " << transmission_walls_1[0].p2.x << ", " << transmission_walls_1[0].p2.y << std::endl;
        std::vector<Wall> transmission_walls_2 = get_transmission_walls(reflection_points[k], direction.p2); 

        std::complex<double> gamma_m = reflexion_coefficient(current_wall, sym_points[k], reflection_points[k]);
        std::complex<double> Tm_1 = transmission_coefficient(direction.p1, reflection_points[k], 
            transmission_walls_1, transmission_walls_1.size()-1);
        std::complex<double> Tm_2 = transmission_coefficient(reflection_points[k], direction.p2, 
            transmission_walls_2, transmission_walls_2.size()-1);
        //std::cout << Tm_1 << std::endl; 
        double GTX = TX.find_GTX(direction.p2);
        field += Tm_1 * Tm_2 * gamma_m * sqrt(60.F * GTX * TX.PTX) * exp(-1.0 * i() * TX.beta * d) / d;
    }
    return field; 
}


std::complex<double> Field::E_double_reflection() {
    std::complex<double> field = 0;
    std::vector<Point> reflection_points{}; 
    std::vector<Point> sym_points{}; 
    get_double_reflection_walls(reflection_points, sym_points);
    //std::cout << double_reflection_walls.size() << std::endl; 
    for (int k = 0; k < double_reflection_walls.size(); k +=2) {
        Wall wall1 = double_reflection_walls[k]; 
        Wall wall2 = double_reflection_walls[k+1];

        //std::cout << wall1.p1.x << ", " << wall1.p1.y << " " << wall1.p2.x << ", " << wall1.p2.y << std::endl; 
        //std::cout << wall2.p1.x << ", " << wall2.p1.y << " " << wall2.p2.x << ", " << wall2.p2.y << std::endl;

       
        const double d = sym_points[k + 1].distance(direction.p2); 
        std::vector<Wall> transmission_walls_1 = get_transmission_walls(direction.p1, reflection_points[k]); 
        std::vector<Wall> transmission_walls_2 = get_transmission_walls(reflection_points[k], reflection_points[k + 1]); 
        std::vector<Wall> transmission_walls_3 = get_transmission_walls(reflection_points[k + 1], direction.p2); 

        ///std::cout << reflection_points[k].x << ", " << reflection_points[k].y << std::endl;
        //std::cout << reflection_points[k+1].x << ", " << reflection_points[k+1].y << std::endl;


        std::complex<double> Tm_1 = transmission_coefficient(direction.p1, reflection_points[k], 
            transmission_walls_1, transmission_walls_1.size()-1);
        //std::cout << transmission_walls_1.size() << std::endl; 
        std::complex<double> Tm_2 = transmission_coefficient(reflection_points[k], reflection_points[k + 1], 
            transmission_walls_2, transmission_walls_2.size()-1);
       /// std::cout << transmission_walls_2.size() << std::endl;
        std::complex<double> Tm_3 = transmission_coefficient(reflection_points[k + 1], direction.p2, 
            transmission_walls_3, transmission_walls_3.size()-1);
       /// std::cout << transmission_walls_3.size() << std::endl;

        std::complex<double> gamma_m1 = reflexion_coefficient(wall1, sym_points[k], reflection_points[k+1]);
        std::complex<double> gamma_m2 = reflexion_coefficient(wall2, sym_points[k+1], direction.p2);
        double GTX = TX.find_GTX(direction.p2);
        field += Tm_1 * Tm_2 * Tm_3* gamma_m1 * gamma_m2 * sqrt(60.F * GTX* TX.PTX) * exp(-1.0 * i() * TX.beta * d) / d;
    }
    return field;
}



#endif