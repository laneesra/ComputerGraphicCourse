//
// Created by agatha on 04.04.18.
//

#ifndef LAB4_POINT_H
#define LAB4_POINT_H
#include <GLFW/glfw3.h>


class Point {
public:
    double x, y;
    Point();
    Point(double x, double y);
    void set_coord(GLsizei width, GLsizei height);
};

#endif //LAB4_POINT_H
