//
// Created by agatha on 19.05.18.
//

#include <math.h>
#include "MyVector.h"


MyVector::MyVector() : x(0), y(0), z(0) {};

MyVector::MyVector(double a, double b, double c) : x(a), y(b), z(c) {};

double MyVector::getX() {
    return x;
};

double MyVector::getY() {
    return y;
};

double MyVector::getZ(){
    return z;
};

void MyVector::countNormal(Point a, Point b, Point c) {
    double x1 = a.getX(), x2 = b.getX(), x3 = c.getX();
    double y1 = a.getY(), y2 = b.getY(), y3 = c.getY();
    double z1 = a.getZ(), z2 = b.getZ(), z3 = c.getZ();

    double X1 = x2 - x1, X2 = x3 - x2;
    double Y1 = y2 - y1, Y2 = y3 - y2;
    double Z1 = z2 - z1, Z2 = z3 - z2;

    double x0 = Y2 * Z1 - Z2 * Y1;
    double y0 = - X2 * Z1 + X1 * Z2;
    double z0 = X2 * Y1 - X1 * Y2;

    x = x0;
    y = y0;
    z = z0;
};

void MyVector::normalize() {
    double length = sqrt(x*x + y*y + z*z);
    x /= length;
    y /= length;
    z /= length;
}