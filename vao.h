#ifndef VAO_H
#define VAO_H
#include "openglIncludes.h"

struct VertexArrayObject{
    VertexArrayObject(float * vertices, int numberOfVertices);
    void Attach();
    void Draw();
    GLuint VAO, VBO;
    int numberOfVertices;
};

#endif //VAO_H
