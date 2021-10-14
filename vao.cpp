#include "vao.h"

VertexArrayObject::VertexArrayObject(float * vertices, int numberOfVertices){
    glGenBuffers(1, &this->VBO);
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this-> VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfVertices, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, 
                          GL_FLOAT, 
                          GL_FALSE, 
                          5 * sizeof(float), 
                          (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void VertexArrayObject::Attach(){
    glBindVertexArray(this->VAO);
}