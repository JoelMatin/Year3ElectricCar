#include "Segment.h"
#include <numeric>


float Segment::slope() {
    return (p2.y - p1.y) / (p2.x - p1.x);
}

bool Segment::segments_intersect(Segment s, Point& p_intersect) {
    double m1 = slope();
    double m2 = s.slope();

    // Vérifier si les segments sont parallèles
    if (m1 == m2) {
        return false;
    }
    double b1, b2, x_inters, y_inters; 
    if (std::isinf(m1)) {
        b1 = p1.x;
        x_inters = b1; 
        b2 = s.p1.y - m2 * s.p1.x;   //m2 pas inf
        y_inters = m2 * b1 + b2;       
        if(x_inters < std::min(s.p1.x, s.p2.x) || x_inters > std::max(s.p1.x, s.p2.x) ||
            y_inters < std::min(p1.y, p2.y) || y_inters > std::max(p1.y, p2.y)) {
            p_intersect = { x_inters, y_inters };
            return false;

        }
    } 
    else if (std::isinf(m2)) {
        b2 = s.p1.x; 
        x_inters = b2; 
        b1 = p1.y - m1 * p1.x;          //m1 pas inf
        y_inters = m1 * b2 + b1;
        if (x_inters < std::min(p1.x, p2.x) || x_inters > std::max(p1.x, p2.x) ||
            (x_inters == p1.x && y_inters == p1.y) || (x_inters == p2.x && y_inters == p2.y) ||
            y_inters < std::min(s.p1.y, s.p2.y) || y_inters > std::max(s.p1.y, s.p2.y) ) {
            p_intersect = { x_inters, y_inters };
            return false;
        }
    }
    else {
        b1 = p1.y - m1 * p1.x;
        b2 = s.p1.y - m2 * s.p1.x;
        x_inters = (b2 - b1) / (m1 - m2);
        y_inters = m1 * x_inters + b1;
        if (x_inters < std::min(p1.x, p2.x) || x_inters > std::max(p1.x, p2.x) ||
        x_inters < std::min(s.p1.x, s.p2.x) || x_inters > std::max(s.p1.x, s.p2.x) ||
            (p1.y == m2 * p1.x + b2) || (p2.y == m2 * p2.x + b2) ) {
            p_intersect = { x_inters, y_inters };
            return false;
        } 
    }   

    p_intersect = { x_inters, y_inters }; 
    return true;
}

