#pragma once
#include "Point.h"
#include <cmath>

class Segment {
public: 
    Point p1;
    Point p2;

    float slope(); 
    bool  segments_intersect(Segment s2, Point& p_intersect); 
};