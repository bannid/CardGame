#ifndef MODEL_H
#define MODEL_H
#include "glmIncludes.h"
#include "shader.h"
#include "vao.h"

#define DRAW_OBJECT3D_FUNCTION(name) void name(Object3D * object, Shader * shader, VertexArrayObject * vao)

struct Object3D{
    glm::vec3 position;
    glm::vec3 scale;
    float rotation;
};

typedef DRAW_OBJECT3D_FUNCTION(DrawObject3DCallback);

void InitalizeObject3D(Object3D * object);
void ScaleObject3D(Object3D * object);
glm::mat4 GetModelMatObject3D(Object3D * object);
DRAW_OBJECT3D_FUNCTION(DrawObject3D);

#endif //MODEL_H
