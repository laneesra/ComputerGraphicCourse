//
// Created by agatha on 19.05.18.
//

#include "Vertex.h"

Vertex::Vertex(Point a) : v(a) {}

Vertex::Vertex() {}

Vertex::Vertex(Point p0, Point p1, Point p2, Point p3) :
        v(p0), p0(p0), p1(p1), p2(p2), p3(p3) {}

void Vertex::addNormal(MyVector normal){
    n.x += normal.getX();
    n.y += normal.getY();
    n.z += normal.getZ();
    n.normalize();
}