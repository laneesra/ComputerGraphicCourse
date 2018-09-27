//
// Created by agatha on 19.05.18.
//

#ifndef LAB6_VERTEX_H
#define LAB6_VERTEX_H

#include <vector>

using namespace std;

#include "Point.h"
#include "MyVector.h"

class Vertex {
public:
    Point v;
    MyVector n;
    Point p0, p1, p2, p3;
    Vertex(Point a);
    Vertex(Point p0, Point p1, Point p2, Point p3);
    void addNormal(MyVector normal);
};


#endif //LAB6_VERTEX_H
