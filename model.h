#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vao.h"
#include "shader.h"

class Object3D{
    public:
    Object3D(const char * name, VertexArrayObject * vao, Shader * shader);
    Object3D(const char * name);
    void AssignVao(VertexArrayObject * vao);
    void AssignShader(Shader * shader);
    void Scale(glm::vec3 scale);
    void Translate(glm::vec3 translation);
    glm::mat4 GetModelMat();
    void Rotate(float angle);
    void Draw();
    glm::vec3 position;
    glm::vec3 scale;
    VertexArrayObject * vao;
    Shader * shader;
    private: 
    const char * name;
    glm::mat4 rotation;
    void Initialize();
};

#endif //MODEL_H
