/*
 * W/S, A/D, Q/E - сдвиг по осям OX, OY, OZ соответственно
 * RIGHT SHIFT - изменение режима отображения модели
 * UP/DOWN, LEFT/RIGHT - вращение вокруг осей OX, OY
 * PG UP/PG DN - масштабирование
 * 1/2 - изменение точности
 */

#include <cstdlib>
#include <cstdio>
#include "Point.h"
#include "Cube.h"
#include "Cylinder.h"

using namespace std;

int accuracy = 50; // Точность отрисовки;
float a = 0.5, b = 0.4, height = 0.8; // Параметры цилиндра;
Cylinder cylinder(a, b, height, accuracy);
float x_centre = 0.f, y_centre = 0.f, z_centre = 0.f; // Координаты центра;
float scale = 1.f; // Масштаб;
float phi = 0.f, ksi = 0.f; // Углы поворота вокруг осей OX, OY;
bool PolygonMode = true; // true - твердотельное отображение модели, false - каркасное;

void m3_per(); // Построение трехточечной перспективы;
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(){
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    GLFWwindow* window = glfwCreateWindow(700, 700, "Lab3", NULL, NULL);
    if (!window) {
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_DEPTH_TEST);
    GLsizei width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    m3_per();
    Cube cube(Point(-0.6, -0.5, 0.0), 0.06);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glScalef(scale, scale, scale);
        glTranslated(x_centre, y_centre, z_centre);
        glRotatef(phi, 0.f, 1.f, 0.f);
        glRotatef(ksi, 1.f, 0.f, 0.f);
        cylinder.draw();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(-40.0f, 0.0f, 1.0f, 0.0f);
        cube.draw();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void m3_per(){
    float p, q, r, c;
    c = 4.f;
    p = q = r = 1/c;
    double M[] = {
            1, 0, 0, p,
            0, 1, 0, q,
            0, 0, 1, r,
            0, 0, 0,  1
    };
    glLoadMatrixd(M);
}

void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    GLfloat angle = 1.f; // угол поворота
    GLfloat d = 0.01f; // расстояние переноса
    GLfloat s = 0.01f; // коэффициент масштабирования
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        Cylinder cylinder1(a, b, height, accuracy++);
        cylinder = cylinder1;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if (accuracy > 3) {
            Cylinder cylinder1(a, b, height, accuracy--);
            cylinder = cylinder1;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        phi += angle;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        phi -= angle;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        ksi += angle;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        ksi -= angle;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        x_centre -= d;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        x_centre += d;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        y_centre += d;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        y_centre -= d;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        z_centre += d;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        z_centre -= d;
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        scale += s;
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        scale -= s;
    }
    if ((key == GLFW_KEY_RIGHT_SHIFT) && (action == GLFW_PRESS)) {
        if (PolygonMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            PolygonMode = false;
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            PolygonMode = true;
        }
    }
}