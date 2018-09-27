//
// Created by agatha on 12.04.18.
//

#include <iostream>
#include <algorithm>
#include "Clipping.h"

using namespace std;

Clipping::Clipping(vector<Point> proc, vector<Point> cut, int mod) : mod(mod) {
    if (!isClockwise(proc))
        proc = do_clockwise(proc);
    if (mod == 3 && isClockwise(cut))
        cut = do_clockwise(cut);
    else if (!isClockwise(cut))
        cut = do_clockwise(cut);

    for (int i = 0; i  < proc.size() - 1; i++)
        processed.emplace_back(proc[i], proc[i + 1]);
    processed.emplace_back(proc.back(), proc[0]);
    for (int i = 0; i < cut.size() - 1; i++)
        cutter.emplace_back(cut[i], cut[i + 1]);
    cutter.emplace_back(cut.back(), cut[0]);

    for (int i = 0; i < processed.size(); i++)
        for (int j = 0; j < cutter.size(); j++)
            find_intersection(i, j);

    set_type_of_intersection();
}


bool Clipping::isClockwise(vector<Point> polygon) {
    auto n = (int)polygon.size();
    double sum = 0;

    for (int i = 0; i < n - 1; i++)
        sum += polygon[i].x * polygon[i + 1].y;
    sum += polygon[n - 1].x * polygon[0].y;

    for (int i = 0; i < n - 1; i++)
        sum -= polygon[i + 1].x * polygon[i].y;
    sum -= polygon[0].x * polygon[n - 1].y;

    return sum >= 0;
}


vector<Point> Clipping::do_clockwise(vector<Point> polygon) {
    vector<Point> correct;
    for (int i = (int)polygon.size() - 1; i >= 0; i--)
        correct.emplace_back(polygon[i]);
    return correct;
}


void Clipping::find_intersection(int i, int j) {
    Edge proc = processed[i];
    Edge cut = cutter[j];
    Point dir1(proc.end.x - proc.begin.x, proc.end.y - proc.begin.y);
    Point dir2(cut.end.x - cut.begin.x, cut.end.y - cut.begin.y);

    double a1 = -dir1.y; double b1 = +dir1.x;
    double d1 = -(a1 * proc.begin.x + b1 * proc.begin.y);

    double a2 = -dir2.y; double b2 = +dir2.x;
    double d2 = -(a2 * cut.begin.x + b2 * cut.begin.y);

    double line1_start = a1 * cut.begin.x + b1 * cut.begin.y + d1;
    double line1_end = a1 * cut.end.x + b1 * cut.end.y + d1;

    double line2_start = a2 * proc.begin.x + b2 * proc.begin.y + d2;
    double line2_end = a2 * proc.end.x + b2 * proc.end.y + d2;

    if (line2_start * line2_end >= 0 || line1_start * line1_end >= 0)
        return;

    double u = line2_start / (line2_start - line2_end);
    Point intersectPoint(proc.begin.x + u * dir1.x, proc.begin.y + u * dir1.y);
    addToEdges(intersectPoint, i, j);
}


void Clipping::addToEdges(Point intersectPoint, int i, int j) {
    intersectPoint.cutterIndex = j;

    int k = find_index(processed, i, intersectPoint);
    processed[i].intersections.insert(processed[i].intersections.begin()+k, intersectPoint);
    processed[i].intersected = true;

    k = find_index(cutter, j, intersectPoint);
    cutter[j].intersections.insert(cutter[j].intersections.begin()+k, intersectPoint);
    cutter[j].intersected = true;
}


int Clipping::find_index(vector<Edge> polygon, int j, Point intersectPoint) {
    int k = 0;
    while (k < polygon[j].intersections.size()) {
        double dist1, dist2;
        dist1 = pow(polygon[j].intersections[k].x - polygon[j].begin.x, 2) + pow(polygon[j].intersections[k].y - polygon[j].begin.y, 2);
        dist2 = pow(intersectPoint.x - polygon[j].begin.x, 2) + pow(intersectPoint.y - polygon[j].begin.y, 2);

        if (dist1 > dist2)
            break;
        k++;
    }

    return k;
}


void Clipping::set_type_of_intersection() {
    for (int i = 0; i < processed.size(); i++) {
        for (int j = 0; j < processed[i].intersections.size(); j++) {
            if (j == 0){
                if (isInside(cutter, processed[i].begin))
                    set_type(2, i, j);
                else set_type(1, i, j);;
            } else {
                if (processed[i].intersections[j-1].type == 2)
                    set_type(1, i, j);
                else set_type(2, i, j);
            }
        }
    }
}


bool Clipping::isInside(vector<Edge> polygon, Point point) {
    int intersections_num = 0;
    int prev = (int)polygon.size() - 1;
    bool prev_under = polygon[prev].begin.y < point.y;

    for (int i = 0; i < polygon.size(); i++) {
        bool cur_under = polygon[i].begin.y < point.y;
        Point a(polygon[prev].begin.x - point.x, polygon[prev].begin.y - point.y);
        Point b(polygon[i].begin.x - point.x, polygon[i].begin.y - point.y);
        double t = (a.x * (b.y - a.y) - a.y * (b.x - a.x));

        if (cur_under && !prev_under && t > 0)
            intersections_num += 1;

        if (!cur_under && prev_under && t < 0)
            intersections_num += 1;

        prev = i;
        prev_under = cur_under;
    }

    return (intersections_num&1) != 0;
}

void Clipping::clip() {
    for (int i = 0; i < processed.size(); i++) {
        for (int j = 0; j < processed[i].intersections.size(); j++) {
            if (mod == 1 && processed[i].intersections[j].type == 1)
                processed_round(i, j);
            else if ((mod == 2 || mod == 3) && processed[i].intersections[j].type == 2)
                processed_round(i, j);
        }
    }

    if (result.empty()) {
        switch (mod) {
            case 1:
                if (isInside(processed, cutter[0].begin))
                    result = cutter;
                else result = processed;
                break;
            case 2:
                if (isInside(processed, cutter[0].begin))
                    result = processed;
                else result = cutter;
                break;
            default:
                for (Edge p : processed)
                    result.emplace_back(p);
                if (isInside(processed, cutter[0].begin))
                    for (Edge c : cutter)
                        result.emplace_back(c);
                break;
        }
    }
}

void Clipping::cutter_round(int i, int j){
    Point intersection;
    if (j != cutter[i].intersections.size() - 1) {
        intersection = cutter[i].intersections[j + 1];
        result.emplace_back(cutter[i].intersections[j], intersection);
    } else {
        result.emplace_back(cutter[i].intersections[j], cutter[i].end);
        int k = i + 1;
        if (k == cutter.size())
            k = 0;
        while (!cutter[k].intersected) {
            result.emplace_back(cutter[k]);
            k++;
            if (k == cutter.size())
                k = 0;
        }
        intersection = cutter[k].intersections[0];
        result.emplace_back(cutter[k].begin, intersection);
    }

}

void Clipping::processed_round(int i, int j){
    int c;
    Point intersection;
    if (j != processed[i].intersections.size() - 1) {
        intersection = processed[i].intersections[j + 1];
        result.emplace_back(processed[i].intersections[j], intersection);
    } else {
        result.emplace_back(processed[i].intersections[j], processed[i].end);
        int k = i + 1;
        if (k == processed.size())
            k = 0;
        while (!processed[k].intersected) {
            result.emplace_back(processed[k]);
            k++;
            if (k == processed.size())
                k = 0;
        }
        intersection = processed[k].intersections[0];
        result.emplace_back(processed[k].begin, intersection);
    }
    c = intersection.cutterIndex;

    int k = 0;
    while (cutter[c].intersections[k].x != intersection.x && cutter[c].intersections[k].y != intersection.y)
        k++;
    cutter_round(c, k);
}


void Clipping::set_type(int type, int i, int j) {
    processed[i].intersections[j].type = type;
}