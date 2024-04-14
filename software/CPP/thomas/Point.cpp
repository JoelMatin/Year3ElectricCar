#include "Point.h"
#include <cmath>


double Point::distance(Point p2) {
    return sqrt(pow(x - p2.x, 2) + pow(y - p2.y, 2));
}
