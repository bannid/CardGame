#include "vao.h"

void LoadVao(VertexArrayObject * vao, float * vertices, int numberOfVertices, bool isDynamic){
    vao->numberOfVertices = numberOfVertices;
    glGenBuffers(1, &vao->VBO);
    glGenVertexArrays(1, &vao->VAO);
    glBindVertexArray(vao->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vao->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfVertices, vertices, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void AttachVao(VertexArrayObject * vao){
    glBindVertexArray(vao->VAO);
}

void DrawVao(VertexArrayObject * vao){
    AttachVao(vao);
    glDrawArrays(GL_TRIANGLES, 0, vao->numberOfVertices / 5);
}