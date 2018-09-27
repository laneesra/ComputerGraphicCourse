//
// Created by agatha on 12.04.18.
//

#include "Edge.h"

Edge::Edge() {}

Edge::Edge(Point a, Point b) {
    begin = a;
    end = b;
    intersected = false;
}

