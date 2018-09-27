//
// Created by agatha on 25.03.18.
//

#include <GL/gl.h>
#include "Cylinder.h"
Cylinder::Cylinder(float a, float b, float height, int accuracy): a(a), b(b), height(height), accuracy(accuracy){
    cyl.clear();
    double h = 0-((double)height)/2;
    while (h < ((double)height)/2){
        Level l(h, a, b, accuracy);
        cyl.push_back(l);
        h += ((double)height)/accuracy;
    }
    Level l(((double)height)/2, a, b, accuracy);
    cyl.push_back(l);
}


void Cylinder::draw(){
    float c1, c2, c3;
    c1 = c2 = 0.49f, c3 = 0.65f;
    glColor3f(c1, c2, c3); // лилово-серый
    c1+=0.001;
    c2+=0.001;
    c3+=0.001;
    //top
    glBegin(GL_TRIANGLE_STRIP);
    for (int j = 0; j < cyl[cyl.size()-1].ps.size(); j++) {
        glVertex3f(cyl[cyl.size() - 1].ps[j].x, cyl[cyl.size() - 1].ps[j].y, cyl[cyl.size() - 1].ps[j].z);
        glVertex3f(0, cyl[cyl.size() - 1].ps[j].y, 0);
    }
    glVertex3f(cyl[cyl.size() - 1].ps[0].x, cyl[cyl.size() - 1].ps[0].y, cyl[cyl.size() - 1].ps[0].z);
    glVertex3f(0, cyl[cyl.size() - 1].ps[0].y, 0);
    glEnd();
    //bottom
    glBegin(GL_TRIANGLE_STRIP);
    for (int j = 0; j < cyl[0].ps.size(); j++) {
        glVertex3f(cyl[0].ps[j].x, cyl[0].ps[j].y, cyl[0].ps[j].z);
        glVertex3f(0, cyl[0].ps[j].y, 0);
    }
    glVertex3f(cyl[0].ps[0].x, cyl[0].ps[0].y, cyl[0].ps[0].z);
    glVertex3f(0, cyl[0].ps[0].y, 0);
    glEnd();
    //side surface
    for (int i = 0; i < cyl.size() - 1; i++) {
        Level l1 = cyl[i], l2 = cyl[i+1];
        vector<Point> ps1, ps2;
        ps1 = l1.ps; ps2 = l2.ps;
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j < ps2.size(); j++){
            glVertex3f(ps1[j].x, ps1[j].y, ps1[j].z);
            glVertex3f(ps2[j].x, ps2[j].y, ps2[j].z);
            c1+=0.00006;
            c2+=0.0001;
            glColor3f(c1, c2, c3);
        }
        glVertex3f(ps1[0].x, ps1[0].y, ps1[0].z);
        glVertex3f(ps2[0].x, ps2[0].y, ps2[0].z);
        glEnd();
    }
}