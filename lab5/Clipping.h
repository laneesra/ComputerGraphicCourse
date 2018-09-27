//
// Created by agatha on 12.04.18.
//

#ifndef LAB5_CLIPPING_H
#define LAB5_CLIPPING_H

#include <vector>
#include "Point.h"
#include "Edge.h"

using namespace std;

class Clipping {
public:
    vector<Edge> processed;
    vector<Edge> cutter;
    vector<Edge> result;
    int mod;
    Clipping(vector<Point> proc, vector<Point> cut, int mod);
    void set_type_of_intersection();
    bool isClockwise(vector<Point> poly);
    vector<Point> do_clockwise(vector<Point> poly);
    void find_intersection(int i, int j);
    void addToEdges(Point intersectPoint, int i, int j);
    bool isInside(vector<Edge> polygon, Point point);
    int find_index(vector<Edge> polygon, int j, Point intersectPoint);
    void clip();
    void cutter_round(int c, int k);
    void processed_round(int i, int j);
    void set_type(int type, int i, int j);
};


#endif //LAB5_CLIPPING_H
