//
// Created by agatha on 07.04.18.
//

#include "YGroup.h"

YGroup::YGroup(Edge e) : edge(e) {
    dx = -((double)(e.end.x-e.begin.x)/(e.end.y-e.begin.y));
    x = e.end.x;
//    dy = e.end.y-e.begin.y - 1;
}