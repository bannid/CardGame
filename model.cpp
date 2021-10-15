#include "model.h"

Object3D::Object3D(const char * name, VertexArrayObject * vao, Shader * shader){
    this->name = name;
    this->AssignVao(vao);
    this->AssignShader(shader);
    this->Initialize();
}

Object3D::Object3D(const char * name){
    this->name = name;
    this->Initialize();
}

void Object3D::AssignVao(VertexArrayObject * vao){
    this->vao = vao;
}

void Object3D::AssignShader(Shader * shader){
    this->shader = shader;
}

void Object3D::Initialize(){
    this->position = glm::vec3(1.0f);
    this->scale = glm::vec3(1.0f);
    this->rotation = glm::mat4(1.0f);
}

glm::mat4 Object3D::GetModelMat(){
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, this->position);
    modelMat = modelMat * this->rotation;
    modelMat = glm::scale(modelMat, glm::vec3(this->scale));
    return modelMat;
}

void Object3D::Scale(glm::vec3 scale){
    this->scale = scale;
}

void Object3D::Translate(glm::vec3 translation){
    this->position += translation;
}

void Object3D::Rotate(float angle){
    this->rotation = glm::rotate(this->rotation, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Object3D::Draw(){
    this->vao->Attach(); 
    this->shader->Attach();
    this->shader->SetMat4("uModelMat", this->GetModelMat());
    this->vao->Draw();
}