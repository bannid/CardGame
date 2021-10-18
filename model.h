#ifndef MODEL_H
#define MODEL_H
#include "glmIncludes.h"
#include "shader.h"
#include "vao.h"

struct Object3D{
    Object3D();
    void Scale(glm::vec3 scale);
    void Translate(glm::vec3 translation);
    glm::mat4 GetModelMat();
    void Rotate(float angle);
    void Draw(Shader * shader, VertexArrayObject * vao);
    glm::vec3 position;
    glm::vec3 scale;
    glm::mat4 rotation;
    void Initialize();
};

#endif //MODEL_H
