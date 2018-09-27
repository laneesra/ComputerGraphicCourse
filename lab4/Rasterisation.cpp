//
// Created by agatha on 04.04.18.
//

#include <algorithm>
#include "Rasterisation.h"
#include "ActiveEdgeList.h"

Rasterisation::Rasterisation(GLFWwindow* window, vector<Point>* v) : vertices(*v) {
    glfwGetFramebufferSize(window, &width, &height);
    buffer = new GLfloat [height*width*3];
    for (int i=0; i<height*width*3; i++)
        buffer[i]=1;
    AELscan();
    glRasterPos2i(-1, -1);
    glDrawPixels(width, height, GL_RGB, GL_FLOAT, buffer);
    delete []buffer;
}

void Rasterisation::AELscan() {
    // создается y-список ребер, упорядоченный по возрастанию
    for (int i=0; i<vertices.size()-1; i++) {
        edges.emplace_back(vertices[i], vertices[i+1]);
    }
    edges.emplace_back(vertices[vertices.size()-1], vertices[0]);
    sort(edges.begin(), edges.end(), cmp);
    ActiveEdgeList AEL;
    // на каждой итерации формируется список активных ребер
    for (int i=(int)edges[edges.size()-1].end.y; i>=edges[0].begin.y; i--) {
        AEL.delete_edges(i); // удаляются ребра, чья нижняя вершина выше текущей строки сканирования

        for (YGroup &activeEdge : AEL.activeEdges) { // выводятся рекурентые соотношения
           // activeEdge.dy--;
            activeEdge.x+=activeEdge.dx;
        }
        for (Edge edge : edges) { // добавляются ребра, начинающиеся на данной строке сканирования
            if (edge.end.y == i) {
                AEL.add_edge(edge);
            }
        }
        AEL.find_intersection(); // вычисляются точки пересечения, отсортированные по возрастанию
        for (int j = 0; AEL.intersections.size()!=0 && j < AEL.intersections.size()-1; j+=2) { // заполняются все промежутки вида [x2i-1; x2i)
            int x = AEL.intersections[j];
            while (x <= AEL.intersections[j + 1]) {
                buffer[(height-i-1)*width*3 + x*3] = 0.49f; // R
                buffer[(height-i-1)*width*3 + x*3 + 1] = 0.49f; // G
                buffer[(height-i-1)*width*3 + x*3 + 2] = 0.65f; // B
                x++;
            }
        }
        AEL.intersections.clear();
    }
}

bool Rasterisation::cmp(Edge &a,Edge &b) {
    return (a.begin.y < b.begin.y);
}

Rasterisation::Rasterisation(GLFWwindow* window) {
    glfwGetFramebufferSize(window, &width, &height);
    glReadBuffer(GL_BACK);
    buffer = new GLfloat [height*width*3];
    for (int i=0; i<height*width*3; i++)
        buffer[i]=1;
    glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, buffer);
    filtration();
    glRasterPos2i(-1, -1);
    glDrawPixels(width, height, GL_RGB, GL_FLOAT, buffer);
    delete []buffer;
}

void Rasterisation::filtration(){
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++) {
            for (int k=0; k<3; k++) {
                GLfloat pixels[9];
                int normalisation = 16;
                pixels[4] = buffer[i*width*3 + j*3 + k];
                if (i==0 || j==0) {
                    pixels[5] = buffer[i*width*3 + (j+1)*3 + k];
                    pixels[7] = buffer[(i+1)*width*3 + j*3 + k];
                    pixels[8] = buffer[(i+1)*width*3 + (j+1)*3 + k];
                    if (i==0) {
                        pixels[0] = pixels[1] = pixels[2] = 1;
                        if (j==0) {
                            pixels[3] = pixels[6] = 1;
                        } else {
                            pixels[3] = buffer[i*width*3 + (j-1)*3 + k];
                            pixels[6] = buffer[(i+1)*width*3 + (j-1)*3 + k];
                        }
                    } else {
                        pixels[3] = pixels[6] = 1;
                        pixels[0] = pixels[3] = pixels[6] = 1;
                        pixels[1] = buffer[(i-1)*width*3 + j*3 + k];
                        pixels[2] = buffer[(i-1)*width*3 + (j+1)*3 + k];
                    }
                } else if (i==height-1 || j==width-1) {
                    pixels[0] = buffer[(i-1)*width*3 + (j-1)*3 + k];
                    pixels[1] = buffer[(i-1)*width*3 + j*3 + k];
                    pixels[3] = buffer[i*width*3 + (j-1)*3 + k];
                    if (i==height-1) {
                        pixels[6] = pixels[7] = pixels[8] = 1;
                        if (j==width-1) {
                            pixels[2] = pixels[5] = 1;
                        } else {
                            pixels[2] = buffer[(i-1)*width*3 + (j+1)*3 + k];
                            pixels[5] = buffer[i*width*3 + (j+1)*3 + k];
                        }
                    } else {
                        pixels[2] = pixels[5] = 1;
                        pixels[6] = buffer[(i+1)*width*3 + (j-1)*3 + k];
                        pixels[7] = buffer[(i+1)*width*3 + j*3 + k];
                        pixels[8] = buffer[(i+1)*width*3 + (j+1)*3 + k];
                    }
                } else {
                    pixels[0] = buffer[(i-1)*width*3 + (j-1)*3 + k];
                    pixels[1] = buffer[(i-1)*width*3 + j*3 + k];
                    pixels[2] = buffer[(i-1)*width*3 + (j+1)*3 + k];
                    pixels[3] = buffer[i*width*3 + (j-1)*3 + k];
                    pixels[5] = buffer[i*width*3 + (j+1)*3 + k];
                    pixels[6] = buffer[(i+1)*width*3 + (j-1)*3 + k];
                    pixels[7] = buffer[(i+1)*width*3 + j*3 + k];
                    pixels[8] = buffer[(i+1)*width*3 + (j+1)*3 + k];
                }
                buffer[i*width*3 + j*3 + k] = (pixels[0] + pixels[1]*2 + pixels[2] +
                                               pixels[3]*2 + pixels[4]*4 + pixels[5]*2 +
                                               pixels[6] + pixels[7]*2 + pixels[8]*1)/normalisation;
            }
        }
    }


}

