//
// Created by agatha on 25.03.18.
//

#ifndef LAB3_CYLINDER_H
#define LAB3_CYLINDER_H

#include <vector>
#include <iostream>
#include <cmath>
#include "Point.h"
#define PI 3.141592653589793238462643383279502884L /* pi */

using namespace std;
struct Level
{
    vector<Point> ps;
    Level(double height, float a, float b, int accuracy) {
        double x, z, phi;
        phi = 0;
        while (phi < 2*PI){
            x = a*cos(phi+2*PI/(accuracy-1));
            z = b*sin(phi+2*PI/(accuracy-1));
            ps.push_back(Point(x, height, z));
            phi+=2*PI/accuracy;
        }
    }
};

class Cylinder {
private:
    float a, b, height; // Параметры цилиндра;
    vector<Level> cyl;
    int accuracy; // точность

public:
    Cylinder(float a, float b, float height, int accuracy);
    void draw();
};


#endif //LAB3_CYLINDER_H
