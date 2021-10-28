#include "model.h"

void Initialize(Object3D * object){
    object->position = glm::vec3(1.0f);
    object->scale = glm::vec3(1.0f);
    object->rotation = 0.0f;
}

glm::mat4 GetModelMatObject3D(Object3D * object){
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

void DrawObject3D(Object3D * object, Shader * shader, VertexArrayObject * vao){
    shader->Attach();
    vao->Attach();
    shader->SetMat4("uModelMat", GetModelMatObject3D(object));
    vao->Draw();
}