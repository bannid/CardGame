#ifndef VAO_H
#define VAO_H
#include "openglIncludes.h"

class VertexArrayObject{
    public:
    VertexArrayObject(float * vertices, int numberOfVertices);
    void Attach();
    void Draw();
    private:
    GLuint VAO, VBO;
    int numberOfVertices;
};

#endif //VAO_H
