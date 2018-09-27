//
// Created by agatha on 25.03.18.
//

#ifndef LAB3_CUBE_H
#define LAB3_CUBE_H
#include <GLFW/glfw3.h>
#include "Point.h"

class Cube {
private:
    Point centre; // координаты центра
    float d; // длина стороны
public:
    Cube(Point c, float d);
    void draw_side(float M[]);
    void draw();
};

#endif //LAB3_CUBE_H
