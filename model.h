#ifndef MODEL_H
#define MODEL_H
#include "glmIncludes.h"
#include "shader.h"
#include "vao.h"

#define DRAW_OBJECT3D_FUNCTION(name) void name(Quad * object, Shader * shader, VertexArrayObject * vao)

struct Quad{
    glm::vec3 position;
    glm::vec3 scale;
    float rotation;
};

typedef DRAW_OBJECT3D_FUNCTION(DrawQuadCallback);

void InitalizeObject3D(Quad * object);
void ScaleObject3D(Quad * object);
glm::mat4 GetModelMatObject3D(Quad * object);
DRAW_OBJECT3D_FUNCTION(DrawQuad);

#endif //MODEL_H
