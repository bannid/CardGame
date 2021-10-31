#ifndef VAO_H
#define VAO_H
#include "openglIncludes.h"

#define LOAD_VAO(name) void name(VertexArrayObject * vao, float * vertices, int numberOfVertices)
#define ATTACH_VAO(name) void name(VertexArrayObject * vao)
#define DRAW_VAO(name) void name(VertexArrayObject * vao)
struct VertexArrayObject{
    unsigned int VAO, VBO;
    int numberOfVertices;
};

typedef LOAD_VAO(LoadVaoCallback);
typedef ATTACH_VAO(AttachVaoCallback);
typedef DRAW_VAO(DrawVaoCallback);
LOAD_VAO(LoadVao);
ATTACH_VAO(AttachVao);
DRAW_VAO(DrawVao);

#endif //VAO_H
