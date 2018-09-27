//
// Created by agatha on 09.04.18.
//

#ifndef LAB5_POINT_H
#define LAB5_POINT_H
#include <GLFW/glfw3.h>


class Point {
public:
    double x, y;
    int type; // 0 - не пересекает, 1 - входит, 2 - выходит
    int cutterIndex;
    Point();
    Point(double x, double y);
    void set_coord(GLsizei width, GLsizei height);
};



#endif //LAB5_POINT_H
