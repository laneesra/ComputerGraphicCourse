//
// Created by agatha on 12.04.18.
//

#ifndef LAB5_EDGE_H
#define LAB5_EDGE_H


#include <vector>
#include "Point.h"
using namespace std;

class Edge {
public:
    Point begin, end;
    vector<Point> intersections;
    bool intersected;
    Edge();
    Edge(Point a, Point b);
};

#endif //LAB5_EDGE_H
