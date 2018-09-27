//
// Created by agatha on 13.05.18.luy

#include "Cylinder.h"

#ifdef DisplayList
bool isFirstList = true;
#endif

Cylinder::Cylinder() {}

#ifdef LightAndTexture
Cylinder::Cylinder(float a, float b, float height, int accuracy, double t, bool light, bool texture): a(a), b(b), height(height),
                                                                                                        accuracy(accuracy), t(t), light(light), texture(texture) {
#else
Cylinder::Cylinder(float a, float b, float height, int accuracy, double t): a(a), b(b), height(height), accuracy(accuracy), t(t) {
#endif
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

#ifdef LightAndTexture
    if (light)
        setNormals();
#else
    setNormals();
#endif

#ifdef DisplayList
    if (isFirstList) isFirstList = false;
    else glDeleteLists(1, 1);

    glNewList(1, GL_COMPILE);
    draw();
    glEndList();
#endif
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
            Point a = top[i].ps[j].v, b = top[i].ps[k].v, c = top[i + 1].ps[j].v;
            MyVector n;
            n.countNormal(a, b, c);
            top[i].ps[j].addNormal(n);
            top[i].ps[k].addNormal(n);
            top[i + 1].ps[j].addNormal(n);
        }
    }

    //bottom
    for (int i = 0; i < bottom.size() - 1; i++) {
        for (int j = 0; j < bottom[i].ps.size(); j++){
            int k = j + 1;
            if (j == bottom[i].ps.size() - 1)
                k = 0;
            Point a = bottom[i].ps[j].v, b = bottom[i].ps[k].v, c = bottom[i + 1].ps[j].v;
            MyVector n;
            n.countNormal(a, c, b);
            bottom[i].ps[j].addNormal(n);
            bottom[i].ps[k].addNormal(n);
            bottom[i + 1].ps[j].addNormal(n);
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
            cyl[i].ps[k].addNormal(n);
            cyl[i + 1].ps[k].addNormal(n);
            cyl[i + 1].ps[j].addNormal(n);
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


void Cylinder::drawSide(vector<Level> &side){
#ifdef VertexArray
    double step = 10/((double)accuracy);
    indices.clear();
    sidens.clear();
    sidevs.clear();
    sidets.clear();
    int k = side[0].ps.size();
    int n = side.size()*k*2;
    for (int i = 0; i < side.size(); i++) {
        for (int j = 0; j < k; j++) {
            Vertex p = side[i].ps[j];
            sidevs.emplace_back(p.v.getX());
            sidevs.emplace_back(p.v.getY());
            sidevs.emplace_back(p.v.getZ());
            sidens.emplace_back(p.n.getX());
            sidens.emplace_back(p.n.getY());
            sidens.emplace_back(p.n.getZ());
            sidets.emplace_back(j*step);
            sidets.emplace_back(i*step);
        }
    }

#ifdef LightAndTexture
    if (light) glEnableClientState(GL_NORMAL_ARRAY);
    if (texture) glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#else
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

    glEnableClientState(GL_VERTEX_ARRAY);

#ifdef LightAndTexture
    if (light) glNormalPointer(GL_DOUBLE, 0, &sidens[0]);
    if (texture) glTexCoordPointer(2, GL_DOUBLE, 0, &sidets[0]);
#else
    glNormalPointer(GL_DOUBLE, 0, &sidens[0]);
    glTexCoordPointer(2, GL_DOUBLE, 0, &sidets[0]);
#endif

    glVertexPointer(3, GL_DOUBLE, 0, &sidevs[0]);

    for (int i = 0; i < side.size() - 1; i++) {
        for (int j = 0; j < k; j++) {
            indices.emplace_back(i*k + j);
            indices.emplace_back((i+1)*k + j);
        }
        indices.emplace_back(i*k);
        indices.emplace_back((i+1)*k);
    }
    glDrawElements(GL_TRIANGLE_STRIP, n, GL_UNSIGNED_INT, &indices[0]);
    glDisableClientState(GL_VERTEX_ARRAY);

#ifdef LightAndTexture
    if (light) glDisableClientState(GL_NORMAL_ARRAY);
    if (texture) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

#else
    glBegin(GL_TRIANGLE_STRIP);
    double step = 10/((double)accuracy);
    for (int i = 0; i < side.size() - 1; i++) {
        for (int j = 0; j < side[i].ps.size(); j++){
            Vertex p = side[i].ps[j], q = side[i + 1].ps[j];
#ifdef LightAndTexture
            if (texture) glTexCoord2d(j * step, i * step);
            if (light) glNormal3d(p.n.getX(), p.n.getY(), p.n.getZ());
            glVertex3d(p.v.getX(), p.v.getY(), p.v.getZ());
            if (texture) glTexCoord2d(j * step, (i + 1) * step);
            if (light) glNormal3d(q.n.getX(), q.n.getY(), q.n.getZ());
            glVertex3d(q.v.getX(), q.v.getY(), q.v.getZ());
        }
        Vertex p = side[i].ps[0], q = side[i + 1].ps[0];
        if (texture) glTexCoord2d(0, i * step);
        if (light) glNormal3d(p.n.getX(), p.n.getY(), p.n.getZ());
        glVertex3d(p.v.getX(), p.v.getY(), p.v.getZ());
        if (texture) glTexCoord2d(0, (i + 1) * step);
        if (light) glNormal3d(q.n.getX(), q.n.getY(), q.n.getZ());
        glVertex3d(q.v.getX(), q.v.getY(), q.v.getZ());
#else

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
#endif
    }
    glEnd();

#endif
}


void Cylinder::drawCircle(vector<Level> &side){
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < side.back().ps.size(); i++){
        double step = 10/((double)accuracy);
        Vertex p = side.back().ps[i];
#ifdef LightAndTexture
        if (texture) glTexCoord2d(side.size() * step, i * step);
        if (light) glNormal3d(p.n.getX(), p.n.getY(), p.n.getZ());
        glVertex3d(p.v.getX(), p.v.getY(), p.v.getZ());
        if (texture) glTexCoord2d(side.size() * step, 0);
        if (light) glNormal3d(0, p.v.getY(), 0);
        glVertex3d(0, p.v.getY(), 0);
#else
        glTexCoord2d(side.size() * step, i * step);
        glNormal3d(p.n.getX(), p.n.getY(), p.n.getZ());
        glVertex3d(p.v.getX(), p.v.getY(), p.v.getZ());
        glTexCoord2d(side.size() * step, 0);
        glNormal3d(0, p.v.getY(), 0);
        glVertex3d(0, p.v.getY(), 0);
#endif
    }
    glEnd();
}