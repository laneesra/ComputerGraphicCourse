//
// Created by agatha on 13.05.18.luy

#include <GL/gl.h>
#include "Cylinder.h"

Cylinder::Cylinder() {}

Cylinder::Cylinder(float a, float b, float height, int accuracy, double t): a(a), b(b), height(height), accuracy(accuracy), t(t) {
    cyl.clear();
    double h = -height/2;
    bottom.emplace_back(h, a, b, accuracy, height);
    top.emplace_back(-h, a, b, accuracy, height);
    cyl.emplace_back(h, a, b, accuracy, height);
    double a1 = a - a/accuracy;
    double b1 = b - b/accuracy;

    while (b1 > 0) {
        bottom.emplace_back(h, a1, b1, accuracy, height);
        top.emplace_back(-h, a1, b1, accuracy, height);
        a1 -= a/accuracy;
        b1 -= b/accuracy;
    }

    while (h < height/2) {
        cyl.emplace_back(h, a, b, accuracy, height);
        h += height/accuracy;
    }

    cyl.emplace_back(height/2, a, b, accuracy, height);
    animation();
    setNormals();
}

void Cylinder::animation() {
    //top
    for (auto &l : top) {
        for (auto &point : l.ps) {
            Point& p0 = point.p0; Point& p1 = point.p1; Point& p2 = point.p2; Point& p3 = point.p3;
            point.v = (1 - t)*(1 - t)*(1 - t)*p0 + 3*t*(1 - t)*(1 - t)*p1 + 3*t*t*(1 - t)*p2 + t*t*t*p3;
        }
    }

    //bottom
    for (auto &l : bottom) {
        for (auto &point : l.ps) {
            Point& p0 = point.p0; Point& p1 = point.p1; Point& p2 = point.p2; Point& p3 = point.p3;
            point.v = (1 - t)*(1 - t)*(1 - t)*p0 + 3*t*(1 - t)*(1 - t)*p1 + 3*t*t*(1 - t)*p2 + t*t*t*p3;
        }
    }

    //cyl
    for (auto &l : cyl) {
        for (auto &point : l.ps) {
            Point& p0 = point.p0; Point& p1 = point.p1; Point& p2 = point.p2; Point& p3 = point.p3;
            point.v = (1 - t)*(1 - t)*(1 - t)*p0 + 3*t*(1 - t)*(1 - t)*p1 + 3*t*t*(1 - t)*p2 + t*t*t*p3;
        }
    }

}

void Cylinder::setNormals(){
    //top
    for (int i = 0; i < top.size() - 1; i++) {
        for (int j = 0; j < top[i].ps.size(); j++){
            int k = j + 1;
            if (j == top[i].ps.size() - 1)
                k = 0;
            Point a = top[i].ps[j].v, b = top[i].ps[k].v, c = top[i + 1].ps[j].v, d = top[i + 1].ps[k].v;
            MyVector n;

            n.countNormal(a, b, c);
            top[i].ps[j].addNormal(n);
            top[i].ps[k].addNormal(n);
            top[i + 1].ps[j].addNormal(n);

            n.countNormal(c, b, d);
            top[i + 1].ps[j].addNormal(n);
            top[i].ps[k].addNormal(n);
            top[i + 1].ps[k].addNormal(n);
        }
    }

    //bottom
    for (int i = 0; i < bottom.size() - 1; i++) {
        for (int j = 0; j < bottom[i].ps.size(); j++){
            int k = j + 1;
            if (j == bottom[i].ps.size() - 1)
                k = 0;
            Point a = bottom[i].ps[j].v, b = bottom[i].ps[k].v, c = bottom[i + 1].ps[j].v, d = bottom[i + 1].ps[k].v;
            MyVector n;

            n.countNormal(a, c, b);
            bottom[i].ps[j].addNormal(n);
            bottom[i].ps[k].addNormal(n);
            bottom[i + 1].ps[j].addNormal(n);

            n.countNormal(c, d, b);
            bottom[i + 1].ps[j].addNormal(n);
            bottom[i].ps[k].addNormal(n);
            bottom[i + 1].ps[k].addNormal(n);
        }
    }
    //side surface
    for (int i = 1; i < cyl.size() - 1; i++) {
        for (int j = 0; j < cyl[i].ps.size(); j++){
            int k = j + 1;
            if (j == cyl[i].ps.size() - 1)
                k = 0;
            Point a = cyl[i].ps[j].v, b = cyl[i].ps[k].v, c = cyl[i + 1].ps[j].v, d = cyl[i + 1].ps[k].v;
            MyVector n;

            n.countNormal(a, b, c);
            cyl[i].ps[j].addNormal(n);
            cyl[i].ps[k].addNormal(n);
            cyl[i + 1].ps[j].addNormal(n);

            n.countNormal(c, b, d);
            cyl[i + 1].ps[j].addNormal(n);
            cyl[i].ps[k].addNormal(n);
            cyl[i + 1].ps[k].addNormal(n);
        }
    }
}


void Cylinder::draw(){
    //top
    drawSide(top);
    drawCircle(top);

    //bottom
    drawSide(bottom);
    drawCircle(bottom);

    //side surface
    drawSide(cyl);
}

double step = 0.2;

void Cylinder::drawSide(vector<Level> &side){
    for (int i = 0; i < side.size() - 1; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j < side[i].ps.size(); j++){
            Vertex p = side[i].ps[j], q = side[i + 1].ps[j];
            glTexCoord2d(j * step, i * step);
            glNormal3d(p.n.getX(), p.n.getY(), p.n.getZ());
            glVertex3d(p.v.getX(), p.v.getY(), p.v.getZ());
            glTexCoord2d(j * step, (i + 1) * step);
            glNormal3d(q.n.getX(), q.n.getY(), q.n.getZ());
            glVertex3d(q.v.getX(), q.v.getY(), q.v.getZ());
        }
        Vertex p = side[i].ps[0], q = side[i + 1].ps[0];
        glTexCoord2d(0, i * step);
        glNormal3d(p.n.getX(), p.n.getY(), p.n.getZ());
        glVertex3d(p.v.getX(), p.v.getY(), p.v.getZ());
        glTexCoord2d(0, (i + 1) * step);
        glNormal3d(q.n.getX(), q.n.getY(), q.n.getZ());
        glVertex3d(q.v.getX(), q.v.getY(), q.v.getZ());
    }
    glEnd();
}

void Cylinder::drawCircle(vector<Level> &side){
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < side.back().ps.size(); i++){
        Vertex p = side.back().ps[i];
        glTexCoord2d(side.size() * step, i * step);
        glNormal3d(p.n.getX(), p.n.getY(), p.n.getZ());
        glVertex3d(p.v.getX(), p.v.getY(), p.v.getZ());
        glTexCoord2d(side.size() * step, 0);
        glNormal3d(0, p.v.getY(), 0);
        glVertex3d(0, p.v.getY(), 0);
    }
    glEnd();
}