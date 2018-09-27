#include <cstdlib>
#include <cstdio>
#include <GLFW/glfw3.h>
#include "Point.h"
#include "Rasterisation.h"

using namespace std;

bool stopDraw, draw, rasterize, filter;
int count1, count2;
GLsizei width, height;
vector<Point> vertices;
Point Begin, End, Current;

void draw_polygon(GLFWwindow* window);
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback (GLFWwindow* window, int button, int action, int mode);
void window_size_callback(GLFWwindow * window, int width , int height);
void reset(GLFWwindow* window);

int main(){
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    GLFWwindow* window = glfwCreateWindow(600, 600, "Lab4", NULL, NULL);
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
        if (draw)
            draw_polygon(window);
        if (rasterize) {
            Rasterisation AELscan(window, &vertices);
            rasterize = false;
        }
        if (filter) {
            Rasterisation filtrate(window);
            filter = false;
        }
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void draw_polygon(GLFWwindow* window){
    glfwGetFramebufferSize(window, &width, &height);
    glColor3f(0.49f, 0.49f, 0.65f); // лилово-серый
    if (draw) {
        if (count1 == 0 && count2==0){
            vertices.emplace_back(Current);
            Current.set_coord(width, height);
            Begin = End = Current;
            count2++;
        } else if (count2==count1 && count2!=0){
            glBegin(GL_LINE_STRIP);
            glVertex2f(End.x, End.y);
            vertices.emplace_back(Current);
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
        draw = false;
    }
}

void reset(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    stopDraw = rasterize = filter = false;
    draw = true;
    if (vertices.size()!=0)
        vertices.clear();
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
    if ((key == GLFW_KEY_RIGHT_SHIFT) && (action == GLFW_PRESS))  {
        rasterize = true; // режим растеризации
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        filter = true; // режим фильтрации
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        reset(window); // сброс
    }
}

void window_size_callback(GLFWwindow* window, int width, int height){
    reset(window);
};