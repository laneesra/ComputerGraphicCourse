//
// Created by agatha on 19.05.18.
//

#ifndef LAB6_MYVECTOR_H
#define LAB6_MYVECTOR_H


#include "Point.h"

class MyVector {
    friend class Vertex;
private:
    double x, y, z;

public:
    MyVector();
    MyVector(double a, double b, double c);
    void countNormal(Point x, Point y, Point z);
    void normalize();
    double getX();
    double getY();
    double getZ();
};


#endif //LAB6_MYVECTOR_H
