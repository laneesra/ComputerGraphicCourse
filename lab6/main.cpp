/*
 * W/S, A/D, Q/E - сдвиг по осям OX, OY, OZ соответственно
 * RIGHT SHIFT - изменение режима отображения модели
 * UP/DOWN, LEFT/RIGHT - вращение вокруг осей OX, OY
 * PG UP/PG DN - масштабирование
 * 1/2 - изменение точности
0 0 0
1 0 0
1 0 0 0
1 0 0 1 */

#include <cstdlib>
#include <cstdio>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include "Cylinder.h"

#define imageWidth 64
#define imageHeight 64
GLubyte Image[imageHeight][imageWidth][4];

using namespace std;

int accuracy = 46; // Точность отрисовки; 46
float a = 0.5, b = 0.4, height = 0.8; // Параметры цилиндра;
Cylinder cylinder;
float x_centre = 0.f, y_centre = 0.f, z_centre = 0.f; // Координаты центра;
float scale = 1.f; // Масштаб;
float phi = 0.f, ksi = 0.f; // Углы поворота вокруг осей OX, OY;
bool PolygonMode = true; // true - твердотельное отображение модели, false - каркасное;
bool AnimationMode = false;
double t = 0;
bool increase = true;
bool on1, on2, on3 = true;
bool on0 = false;
bool TextureMode = false;
GLuint texture;

void m3_per(); // Построение трехточечной перспективы;
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void animation();
void lighting();
void texturing();
void light_on(int i);
void readFromFile();
void writeToFile();
void makeTexture();

void init() {
  //  readFromFile();
    cylinder = Cylinder(a, b, height, accuracy, t);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    makeTexture();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

int main(){
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    GLFWwindow* window = glfwCreateWindow(700, 700, "Lab6", NULL, NULL);
    if (!window) {
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);

    GLsizei width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    init();
    m3_per();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glScalef(scale, scale, scale);
        glTranslated(x_centre, y_centre, z_centre);
        glRotatef(phi, 0.f, 1.f, 0.f);
        glRotatef(ksi, 1.f, 0.f, 0.f);
        animation();
        texturing();
        cylinder.draw();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(-40.0f, 0.0f, 1.0f, 0.0f);
        lighting();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void lighting(){
    if (!on0)
        glDisable(GL_LIGHT0);
    else light_on(0);
    if (!on1)
        glDisable(GL_LIGHT1);
    else light_on(1);
    if (!on2)
        glDisable(GL_LIGHT2);
    else light_on(2);
    if (!on3)
        glDisable(GL_LIGHT3);
    else light_on(3);
}

void animation(){
    double coef = 0.009;
    if (AnimationMode){
        if (increase)
            t += coef;
        else t -= coef;
        if (t >= 1 || t <= 0)
            increase = !increase;
        cylinder = Cylinder(a, b, height, accuracy, t);
    }
}

void light_on(int i){
    switch (i) {
        case 0: {
            GLfloat light0_diffuse[] = {1, 0, 0};
            GLfloat light0_position[] = {-1.0, 0.0, 0.0, 1.0};
            glEnable(GL_LIGHT0);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
            glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
            break;
        }
        case 1: {
            GLfloat light1_diffuse[] = {0, 1, 0};
            GLfloat light1_position[] = {0.0, 1.0, 0.0, 1.0};
            glEnable(GL_LIGHT1);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
            glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
            break;
        }
        case 2: {
            GLfloat light2_diffuse[] = {0, 0, 1};
            GLfloat light2_position[] = {0.0, 0.0, -1.0, 1.0};
            glEnable(GL_LIGHT2);
            glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
            glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
            break;
        }
        case 3: {
            GLfloat light3_diffuse[] = {1, 1, 0};
            GLfloat light3_position[] = {0.0, -1.0, 0.0, 1.0};
            glEnable(GL_LIGHT3);
            glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
            glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
            break;
        }
    }
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

void texturing(){
    if (!TextureMode) {
        glShadeModel(GL_SMOOTH);
        glDisable(GL_TEXTURE_2D);
    } else {
        glShadeModel(GL_FLAT);
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}

void makeTexture() {
    int i, j, c;
    for (i = 0; i < imageHeight; i++) {
        for (j = 0; j < imageWidth; j++) {
            c=(((i&0x8)==0)^((j&0x8)==0))*255;
            Image[i][j][0]=(GLubyte)c;
            Image[i][j][1]=(GLubyte)c;
            Image[i][j][2]=(GLubyte)c;
            Image[i][j][3]=(GLubyte)255;
        }
    }
}

void writeToFile() {
    ofstream fout("../data.txt");

    if (!fout.is_open()) {
        cout << "Can't read from filer" << endl;
        return;
    }

    fout << accuracy << endl;
    fout << x_centre << " " << y_centre << " " << z_centre << endl; // Координаты центра;
    fout << scale << " " << phi << " " << ksi << endl; // Масштаб; Углы поворота вокруг осей OX, OY;
    fout << PolygonMode << " " << TextureMode << " " << AnimationMode << " " << t << endl;
    fout << on0 << " " << on1 << " " << on2 << " " <<  on3 << endl; // Парамтеры света;

    fout.close();
}

void readFromFile() {
    ifstream fin("../data.txt");

    if (!fin.is_open()) {
        cout << "Can't write to file" << endl;
        return;
    }

    fin >> accuracy;
    fin >> x_centre >> y_centre >> z_centre; // Координаты центра;
    fin >> scale >> phi >> ksi; // Масштаб; Углы поворота вокруг осей OX, OY;
    fin >> PolygonMode >> TextureMode >> AnimationMode >> t;
    fin >> on0 >> on1 >> on2 >> on3; // Парамтеры света;

    cylinder = Cylinder(a, b, height, accuracy, t);
    texturing();
    fin.close();
}


void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    GLfloat angle = 1.f; // угол поворота
    GLfloat d = 0.01f; // расстояние переноса
    GLfloat s = 0.01f; // коэффициент масштабирования
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        readFromFile();
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        writeToFile();
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        texturing();
        TextureMode = !TextureMode;
    }
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        cylinder = Cylinder(a, b, height, accuracy++, t);
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
        if (accuracy > 3)
            cylinder = Cylinder(a, b, height, accuracy--, t);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        on0 = !on0;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        on1 = !on1;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        on2 = !on2;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        on3 = !on3;
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        AnimationMode = !AnimationMode;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        phi -= angle;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        phi += angle;
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
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        scale += s;
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        scale -= s;
    }
    if ((key == GLFW_KEY_RIGHT_SHIFT) && (action == GLFW_PRESS)) {
        if (PolygonMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        PolygonMode = !PolygonMode;
    }
}