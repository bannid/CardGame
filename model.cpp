#include "model.h"

void Initialize(Quad * object){
    object->position = glm::vec3(1.0f);
    object->scale = glm::vec3(1.0f);
    object->rotation = 0.0f;
}

glm::mat4 GetModelMatObject3D(Quad * object){
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, object->position);
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 rot = glm::rotate(identity,
                                glm::radians(object->rotation),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    modelMat = modelMat * rot;
    modelMat = glm::scale(modelMat, glm::vec3(object->scale));
    return modelMat;
}

void DrawQuad(Quad * object, Shader * shader, VertexArrayObject * vao){
    AttachShader(shader);
    AttachVao(vao);
    SetMat4Shader(shader, "uModelMat", GetModelMatObject3D(object));
    DrawVao(vao);
}