//
// Created by agatha on 05.04.18.
//

#include "Edge.h"

Edge::Edge() {}

Edge::Edge(Point a, Point b) {
    if (a.y > b.y) {
        begin = b; end = a;
    } else {
      begin = a, end = b;
    }
}