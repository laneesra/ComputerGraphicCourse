//
// Created by agatha on 04.04.18.
//

#include "Point.h"

Point::Point(){}

Point::Point(double x, double y) : x(x), y(y) {}

void Point::set_coord(GLsizei width, GLsizei height) {
    x = x/(width/2)-1;
    y = -y/(height/2)+1;
}
