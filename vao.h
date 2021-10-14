#ifndef VAO_H
#define VAO_H

#include <glad.h>
#include <glfw3.h>

class VertexArrayObject{
    public:
    VertexArrayObject(float * vertices, int numberOfVertices);
    void Attach();
    private:
    GLuint VAO, VBO;
};

#endif //VAO_H
