
#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

class Cube {
public:
    Cube(double size);
    void draw(vector<GLint>& shaders);
    ~Cube();

private:

    GLuint *VBO, *VAO, EBO;

    double size;
    void draw_edge(double *edge, double* colour);
    void initDataOfEdge(int index, double* data, int len);
};