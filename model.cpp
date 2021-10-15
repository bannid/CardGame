#include "model.h"

Object3D::Object3D(const char * name, VertexArrayObject * vao){
    this->name = name;
    this->AssignVao(vao);
    this->Initialize();
}

Object3D::Object3D(const char * name){
    this->name = name;
    this->Initialize();
}

void Object3D::AssignVao(VertexArrayObject * vao){
    this->vao = vao;
}

void Object3D::Initialize(){
    this->position = glm::vec3(1.0f);
    this->scale = glm::vec3(1.0f);
    this->rotation = 0.0f;
}

glm::mat4 Object3D::GetModelMat(){
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, this->position);
    modelMat[0].x = this->scale.x;
    modelMat[1].y = this->scale.y;
    modelMat[2].z = this->scale.z;
    return modelMat;
}

void Object3D::Scale(glm::vec3 scale){
    this->scale = scale;
}

void Object3D::Translate(glm::vec3 translation){
    this->position += translation;
}

void Object3D::Rotate(float angle){
    this->rotation += angle;
}