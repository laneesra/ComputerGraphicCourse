#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
GLfloat x = 0.f, y = 0.f, z = 0.f; // Координаты центра;
GLfloat sc = 1.f; // Масштаб;
GLfloat phi = 0.f, ksi = 0.f; // Углы поворота вокруг осей OX, OY;
bool PolygonMode = true; // true - твердотельное отображение модели, false - каркасное;
const GLuint WIDTH = 800, HEIGHT = 800;
GLuint VBO, VAO;
glm::mat4 M;
glm::mat4 MVP;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void make_cube(double); // Построение куба;
void make_matrix();
void m3_per(); // Построение трехточечной перспективы;
void error_callback(int error, const char* description);

// Шейдеры
const GLchar* vertexShaderSource = "#version 330\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 color;\n"
        "out vec3 fragmentColor;\n"
        "uniform mat4 MVP;\n"
        "void main()\n"
        "{\n"
        "gl_Position = MVP * vec4(position, 1.0f);\n"
        "fragmentColor = color;"
        "}\0";

const GLchar* fragmentShaderSource = "#version 330\n"
        "in vec3 fragmentColor;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(fragmentColor, 1.0);\n"
        "}\0";
glm::mat4 R(1.0f);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lab8", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetErrorCallback(error_callback);
    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    GLint success;
    GLchar infoLog[512];
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER PROGRAM LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    m3_per();
    GLuint matrix = glGetUniformLocation(shaderProgram, "MVP");
    make_cube(0.3f);
    make_matrix();

    R = glm::translate(M, glm::vec3(-0.5, -0.5, 0.5));
    R = glm::scale(R, glm::vec3(0.2, 0.2, 0.2));

    glUseProgram(shaderProgram);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniformMatrix4fv(matrix, 1, GL_FALSE, &MVP[0][0]);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUniformMatrix4fv(matrix, 1, GL_FALSE, &R[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void m3_per(){
    float p, q;
    p = q = 1/3.f;
    M[0] = {1.f, 0.f, 0.f, p};
    M[1] = {0.f, 1.f, 0.f, q};
    M[2] = {0.f, 0.f, 1.f, 0};
    M[3] = {0.f, 0.f, 0.f, 1};
}

void make_cube(double size){
    GLdouble vertices[] = {
            -size,-size,-size, 1., 1., 0.,
            -size,-size, size, 0., 0., 1.,
            -size, size, size, 1., 0., 0.,
            size, size,-size, 1., 1., 0.,
            -size,-size,-size, 0., 0., 1.,
            -size, size,-size, 1., 0., 0.,

            size,-size, size, 1., 0., 0.,
            -size,-size,-size, 0., 0., 1.,
            size,-size,-size, 1., 1., 0.,
            size, size,-size, 0., 0., 1.,
            size,-size,-size, 1., 0., 0.,
            -size,-size,-size, 1., 1., 0.,

            -size,-size,-size, 1., 0., 0.,
            -size, size, size, 0., 0., 1.,
            -size, size,-size, 1., 0., 1.,
            size,-size, size, 1., 0., 0.,
            -size,-size, size, 0., 0., 1.,
            -size,-size,-size, 1., 0., 1.,

            -size, size, size, 1., 0., 0.,
            -size,-size, size, 0., 1., 0.,
            size,-size, size, 1., 1., 1.,
            size, size, size, 1., 0., 0.,
            size,-size,-size, 0., 1., 0.,
            size, size,-size, 1., 1., 1.,

            size,-size,-size, 1., 0., 1.,
            size, size, size, 1., 1., 0.,
            size,-size, size, 1., 1., 1.,
            size, size, size, 1., 0., 1.,
            size, size,-size, 1., 1., 0.,
            -size, size,-size, 1., 1., 1.,

            size, size, size, 1., 0., 1.,
            -size, size,-size, 1., 1., 0.,
            -size, size, size, 1., 0., 0.,
            size, size, size, 1., 0., 1.,
            -size, size, size, 1., 1., 0.,
            size,-size, size, 1., 0., 0.,
    };
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO); //получение идентификатора буфера
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //выбор активного буфера
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), (GLvoid*)(3* sizeof(GLdouble)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void make_matrix(){
    MVP = glm::scale(M, glm::vec3(sc, sc, sc));
    MVP = glm::translate(MVP, glm::vec3(x, y, z));
    MVP = glm::rotate(MVP, phi, glm::vec3(0.0, 1.0, 0.0));
    MVP = glm::rotate(MVP, ksi, glm::vec3(1.0, 0.0, 0.0));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    GLfloat angle = 1.f; // угол поворота
    GLfloat d = 0.01f; // расстояние переноса
    GLfloat s = 0.01f; // коэффициент масштабирования

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        phi += angle;
        int width, height;
        glfwGetWindowSize(window, &width, &height ); // получение размеров окна
        height = height > 0 ? height : 1; // защищаемся от деления на ноль
        glViewport( 0, 0, width, height );
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
        x -= d;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        x += d;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        y += d;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        y -= d;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        z += d;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        z -= d;
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        sc += s;
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        sc -= s;
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
    make_matrix();
}