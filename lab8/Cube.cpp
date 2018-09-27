#include "Cube.h"

void Cube::initDataOfEdge(int index, double* data, int len) {
    glBindVertexArray(VAO[index]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
    glBufferData(GL_ARRAY_BUFFER, len, data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Cube::Cube(double size) {
    this->size = size;

    VBO = new GLuint[6];
    VAO = new GLuint[6];

    glGenVertexArrays(6, VAO);
    glGenBuffers(6, VBO);

    double top[] = {
            size,  size,  size,
            size, -size,  size,
            -size,  size,  size,
            -size, -size,  size
    };
    double down[] = {
            size, -size, -size,
            size,  size, -size,
            -size, -size, -size,
            -size,  size, -size
    };
    double front[] = {
            size, -size,  size,
            size, -size, -size,
            -size, -size,  size,
            -size, -size, -size
    };
    double back[] = {
            -size,  size,  size,
            -size,  size, -size,
            size,  size,  size,
            size,  size, -size
    };
    double left[] = {
            -size,  size,  size,
            -size,  size, -size,
            -size, -size,  size,
            -size, -size, -size
    };
    double right[] = {
            size, -size, -size,
            size, -size,  size,
            size,  size, -size,
            size,  size,  size
    };


    VBO = new GLuint[6];
    VAO = new GLuint[6];

    glGenVertexArrays(6, VAO);
    glGenBuffers(6, VBO);


    initDataOfEdge(0, top, sizeof(top));
    initDataOfEdge(1, down, sizeof(down));
    initDataOfEdge(2, right, sizeof(right));
    initDataOfEdge(3, left, sizeof(left));
    initDataOfEdge(4, front, sizeof(front));
    initDataOfEdge(5, back, sizeof(back));
}

void Cube::draw(vector<GLint>& shaders) {

    glUseProgram(shaders[0]);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glUseProgram(shaders[1]);
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glUseProgram(shaders[2]);
    glBindVertexArray(VAO[2]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glUseProgram(shaders[3]);
    glBindVertexArray(VAO[3]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glUseProgram(shaders[4]);
    glBindVertexArray(VAO[4]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glUseProgram(shaders[5]);
    glBindVertexArray(VAO[5]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Cube::draw_edge(double* vertecies, double* colour) {
    glColor3d(colour[0], colour[1], colour[2]);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, vertecies);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

Cube::~Cube() {
    glDeleteVertexArrays(6, VAO);
    glDeleteBuffers(6, VBO);
    delete[] VBO;
    delete[] VAO;
}