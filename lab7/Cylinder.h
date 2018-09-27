//
// Created by agatha on 13.05.18.
//

#ifndef LAB6_CYLINDER_H
#define LAB6_CYLINDER_H

#include <vector>
#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include "Point.h"
#include "Vertex.h"
#include "defs.h"

#define PI 3.1415926535897/* pi */

using namespace std;

struct Level
{
    vector<Vertex> ps;
    Level(double h, double a, double b, int accuracy, double height) {
        double x, z, phi, coef;
        Point p0, p1, p2, p3;
        coef = 1 - abs((h + height / 2) / height);
        phi = 0;
        while (phi < 2 * PI) {
            x = a * cos(phi);
            z = b * sin(phi);
            p0 = Point(x, h, z);
            if (h != - height / 2){
                p1 = Point(x - x * 2 * (1 - coef) / 3, h, z - z * 2 * (1 - coef) / 3);
                p2 = Point(x - x * (1 - coef) / 3, h, z - z * (1 - coef) / 3);
            } else {
                p1 = Point(x, h, z);
                p2 = Point(x, h, z);
            }

            p3 = Point(x * coef, h, z * coef);
            ps.emplace_back(p0, p1, p2, p3);
            phi += 2 * PI / accuracy;
        }
    }
};

class Cylinder {
private:
#ifdef VertexArray
    vector<GLuint> indices;
    vector<double> sidevs;
    vector<double> sidens;
    vector<double> sidets;
#endif

    float a, b, height; // Параметры цилиндра;
    vector<Level> cyl;
    vector<Level> top;
    vector<Level> bottom;
    int accuracy; // точность
    double t;

public:
#ifdef LightAndTexture
    bool light;
    bool texture;
#endif

    Cylinder();
#ifdef LightAndTexture
    Cylinder(float a, float b, float height, int accuracy, double t, bool light, bool texture);
#else
    Cylinder(float a, float b, float height, int accuracy, double t);
#endif
    void setNormals();
    void animation();
    void draw();
    void drawSide(vector<Level> &side);
    void drawCircle(vector<Level> &side);
};


#endif //LAB6_CYLINDER_H
