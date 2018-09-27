//
// Created by agatha on 13.05.18.
//

#ifndef LAB6_POINT_H
#define LAB6_POINT_H


class Point{
private:
    double x, y, z;
    double coord[3];

public:
    Point(double x, double y, double z);
    Point();
    friend Point operator+(const Point& left, const Point& right);
    friend Point& operator+=(Point& left, const Point& right);
    friend Point operator*(const Point& left, const Point& right);
    friend Point operator*(double t, const Point& v);
    friend Point operator-(const Point &v);
    friend Point operator+(double t, const Point &v);
    friend Point operator+(const Point &v, double t);
    double getX();
    double getY();
    double getZ();
    double* getCoord();
};


#endif //LAB6_POINT_H
