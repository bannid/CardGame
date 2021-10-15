#ifndef CARD_H
#define CARD_H

#include "glmIncludes.h"
#include "common.h"
#include "model.h"
#include "shader.h"
#include "vao.h"

class Card{
    public:
    Card(CardSuit suit, CardRank cardRank, glm::vec3 position);
    Card();
    void Initialize();
    void SetSuit(CardSuit suit);
    void SetRank(CardRank cardRank);
    void SetPosition(glm::vec3 position);
    void Draw(Shader * shader, VertexArrayObject * vao);
    CardSuit suit;
    CardRank cardRank;
    glm::vec3 position;
    Object3D front;
    Object3D back;
    bool flipped;
};

#endif //CARD_H
