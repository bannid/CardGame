#ifndef CARD_H
#define CARD_H

#include "glmIncludes.h"
#include "common.h"
#include "model.h"
#include "shader.h"
#include "vao.h"

class Card{
    public:
    Card(CardSuit suit,
         CardRank cardRank,
         glm::vec3 position,
         glm::vec3 size);
    Card();
    void Initialize();
    void SetSuit(CardSuit suit);
    void SetRank(CardRank cardRank);
    void SetPosition(glm::vec3 position);
    void SetScale(glm::vec3 scale);
    void Rotate(float angle);
    CardSuit suit;
    CardRank cardRank;
    glm::vec3 position;
    glm::vec3 scale;
    Object3D front;
    Object3D back;
    bool flipped;
};

#endif //CARD_H
