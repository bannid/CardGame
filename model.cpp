#include "model.h"

Object3D::Object3D(){
    this->Initialize();
}
void Object3D::Initialize(){
    this->position = glm::vec3(1.0f);
    this->scale = glm::vec3(1.0f);
    this->rotation = 0.0f;
}

glm::mat4 Object3D::GetModelMat(){
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, this->position);
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 rot = glm::rotate(identity,
                                glm::radians(this->rotation),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    modelMat = modelMat * rot;
    modelMat = glm::scale(modelMat, glm::vec3(this->scale));
    return modelMat;
}

void Object3D::Scale(glm::vec3 scale){
    this->scale = scale;
}

void Object3D::Translate(glm::vec3 translation){
    this->position += translation;
}

void Object3D::Draw(Shader * shader, VertexArrayObject * vao){
    shader->Attach();
    vao->Attach();
    shader->SetMat4("uModelMat", this->GetModelMat());
    vao->Draw();
}