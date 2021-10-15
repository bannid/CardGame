#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vao.h"

class Object3D{
    public:
    Object3D(const char * name, VertexArrayObject * vao);
    Object3D(const char * name);
    void AssignVao(VertexArrayObject * vao);
    void Scale(glm::vec3 scale);
    void Translate(glm::vec3 translation);
    glm::mat4 GetModelMat();
    void Rotate(float angle);
    glm::vec3 position;
    glm::vec3 scale;
    VertexArrayObject * vao;
    private: 
    const char * name;
    float rotation;
    void Initialize();
};

#endif //MODEL_H
