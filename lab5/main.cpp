#include <cstdlib>
#include <cstdio>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include "Point.h"
#include "Clipping.h"

using namespace std;

bool drawProcessed, drawCutter, stopDraw, intersection, merger, difference;
int count1, count2;
GLsizei width, height;
vector<Point> processed;
vector<Point> cutter;
Point Begin, End, Current;

void draw_polygon_by_step(GLFWwindow* window, vector<Point>* polygon);
void draw_polygon(vector<Point> polygon);
void draw_result(GLFWwindow* window, vector<Edge> result);
void draw_again();
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback (GLFWwindow* window, int button, int action, int mode);
void window_size_callback(GLFWwindow * window, int width , int height);
void reset(GLFWwindow* window);

int main(){
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    GLFWwindow* window = glfwCreateWindow(600, 600, "Lab5", NULL, NULL);
    if (!window) {
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(error_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    reset(window);

    while (!glfwWindowShouldClose(window)) {
        if (drawProcessed)
            draw_polygon_by_step(window, &processed);
        if (drawCutter)
            draw_polygon_by_step(window, &cutter);
        if (intersection){
            Clipping c(processed, cutter, 1);
            c.clip();
            draw_result(window, c.result);
            intersection = false;
        }
        if (merger){
            Clipping c(processed, cutter, 2);
            c.clip();
            draw_result(window, c.result);
            merger = false;
        }
        if (difference){
            Clipping c(processed, cutter, 3);
            c.clip();
            draw_result(window, c.result);
            difference = false;
        }
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}


void draw_polygon(vector<Point> polygon){
    glBegin(GL_LINE_LOOP);
    for (int i=0; i<polygon.size(); i++){
        polygon[i].set_coord(width, height);
        glVertex2f(polygon[i].x, polygon[i].y);
    }
    glEnd();
}

void draw_again(){
    glColor3f(1.f, 0.49f, 0.65f);
    draw_polygon(processed);
    glColor3f(0.49f, 0.49f, 0.65f);
    draw_polygon(cutter);
}

void draw_result(GLFWwindow* window, vector<Edge> result){
    glColor3f(0, 1, 0);
    glfwGetFramebufferSize(window, &width, &height);
    for (Edge e : result) {
        e.begin.set_coord(width, height);
        e.end.set_coord(width, height);
        glBegin(GL_LINE_STRIP);
        glVertex2f(e.begin.x, e.begin.y);
        glVertex2f(e.end.x, e.end.y);
        glEnd();
    }
}

void draw_polygon_by_step(GLFWwindow* window, vector<Point>* polygon){
    glfwGetFramebufferSize(window, &width, &height);
    if (drawCutter)
        glColor3f(0.49f, 0.49f, 0.65f); // лилово-серый
    else glColor3f(1.f, 0.49f, 0.65f); // розовый
    if (drawCutter || drawProcessed) {
        if (count1 == 0 && count2 == 0){
            polygon->emplace_back(Current);
            Current.set_coord(width, height);
            Begin = End = Current;
            count2++;
        } else if (count2==count1 && count2!=0){
            glBegin(GL_LINE_STRIP);
            glVertex2f(End.x, End.y);
            polygon->emplace_back(Current);
            Current.set_coord(width, height);
            End = Current;
            glVertex2f(Current.x, Current.y);
            glEnd();
            count2++;
        }
    }
    if (stopDraw) {
        glBegin(GL_LINE_STRIP);
        glVertex2f(End.x, End.y);
        glVertex2f(Begin.x, Begin.y);
        glEnd();
        if (drawProcessed) {
            drawProcessed = false;
            drawCutter = true;
            stopDraw = false;
            count1 = -1; count2 = 0;
        } else if (drawCutter) {
            drawCutter = false;
        }
    }
}


void reset(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    stopDraw = drawCutter = false;
    drawProcessed = true;
    intersection = merger = difference = false;
    processed.clear();
    cutter.clear();
    count1 = -1; count2 = 0;
}

void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mode) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        glfwGetCursorPos(window, &Current.x, &Current.y);
        count1++;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        stopDraw = true; // конец отрисовки
    }
    if ((key == GLFW_KEY_1) && (action == GLFW_PRESS)){
        intersection = true;
    }
    if ((key == GLFW_KEY_2) && (action == GLFW_PRESS))  {
        merger = true;
    }
    if ((key == GLFW_KEY_3) && (action == GLFW_PRESS))  {
        difference = true;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        reset(window); // сброс
    }
    if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS) {
        draw_again();
    }
}

void window_size_callback(GLFWwindow* window, int width, int height){
    reset(window);
};