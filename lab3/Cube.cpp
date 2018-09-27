//
// Created by agatha on 25.03.18.
//

#include "Cube.h"

Cube::Cube(Point c, float d): centre(c), d(d) {}

void Cube::draw_side(float M[]){
    glBegin(GL_QUADS);
    glVertex3f(M[0], M[1], M[2]);
    glVertex3f(M[3], M[4], M[5]);
    glVertex3f(M[6], M[7], M[8]);
    glVertex3f(M[9], M[10], M[11]);
    glEnd();
}

void Cube::draw() {
    float ps[] = {0, centre.x - d, centre.x + d,
                  centre.y - d, centre.y + d,
                  centre.z - d, centre.z + d};

    float front[] = {ps[1], ps[3], ps[6], ps[2], ps[3], ps[6], ps[2], ps[4], ps[6], ps[1], ps[4], ps[6]};
    float back[] = {ps[1], ps[3], ps[5], ps[2], ps[3], ps[5], ps[2], ps[4], ps[5], ps[1], ps[4], ps[5]};
    float bottom[] = {ps[1], ps[3], ps[6], ps[2], ps[3], ps[6], ps[2], ps[3], ps[5], ps[1], ps[3], ps[5]};
    float top[] = {ps[1], ps[4], ps[6], ps[2], ps[4], ps[6], ps[2], ps[4], ps[5], ps[1], ps[4], ps[5]};
    float left[] = {ps[1], ps[3], ps[5], ps[1], ps[3], ps[6], ps[1], ps[4], ps[6], ps[1], ps[4], ps[5]};
    float right[] = {ps[2], ps[3], ps[6], ps[2], ps[3], ps[5], ps[2], ps[4], ps[5], ps[2], ps[4], ps[6]};

    glColor3f(0.51f, 0.7f, 0.86f); // голубой
    draw_side(back);
    glColor3f(0.68f, 0.93f, 0.93f); // синий PaleTurquoise
    draw_side(front);
    glColor3f(0.49f, 0.49f, 0.65f); // лилово-серый
    draw_side(left);
    glColor3f(0.6f, 0.6f, 0.6f); // серый
    draw_side(right);
    glColor3f(0.9f, 1.f, 1.f); // почти белый
    draw_side(bottom);
    glColor3f(0.4f, 0.8f, 0.66f); // MediumAquamarine
    draw_side(top);
}
