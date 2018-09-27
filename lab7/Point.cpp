//
// Created by agatha on 13.05.18.
//

#include "Point.h"

Point::Point(double x, double y, double z): x(x), y(y), z(z) {
    coord[0] = x; coord[1] = y; coord[2] = z;
};

Point::Point() {};

double Point::getX() {
    return x;
};

double Point::getY() {
    return y;
};

double Point::getZ(){
    return z;
};

double* Point::getCoord(){
    return coord;
};

Point operator+(const Point& left, const Point& right) {
    double x = left.x + right.x, y = left.y + right.y, z = left.z + right.z;
    return Point(x, y, z);
};

Point operator*(double t, const Point& v) {
    double x = v.x * t, y = v.y * t, z = v.z * t;
    return Point(x, y, z);
}

Point operator*(const Point& left, const Point& right) {
    double x = left.x * right.x, y = left.y * right.y, z = left.z * right.z;
    return Point(x, y, z);
}

Point operator-(const Point &v) {
    return Point(-v.x, -v.y, -v.z);
}

Point operator+(double t, const Point &v) {
    return Point(t + v.x, t + v.y, t + v.z);
}

Point operator+(const Point &v, double t) {
    return Point(t + v.x, t + v.y, t + v.z);
}

Point& operator+=(Point& left, const Point& right) {
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
    return left;
};