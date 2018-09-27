//
// Created by agatha on 04.04.18.
//

#ifndef LAB4_RASTERISATION_H
#define LAB4_RASTERISATION_H

#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>
#include "Point.h"
#include "Edge.h"

using namespace std;

class Rasterisation {
private:
    vector<Point> vertices;
    vector<Edge> edges;
    GLfloat* buffer;
    GLsizei width, height;
public:
    Rasterisation(GLFWwindow* window); // фильтрация
    Rasterisation(GLFWwindow* window, vector<Point>* vertices); // закраска
    void AELscan();
    void filtration();
    static bool cmp(Edge &a,Edge &b);
};


#endif //LAB4_RASTERISATION_H
