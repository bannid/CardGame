#ifndef CARD_H
#define CARD_H

#include "cardTypes.h"
#include "glmIncludes.h"
#include "animators.h"

struct Card{
    Suit suit;
    Rank rank;
    glm::vec3 position;
    glm::vec3 scale;
    bool isFlipped;
    AnimationRotate rotateAnimation;
    float rotateY = 0.0f;
    glm::mat4 transform;
    Card();
    Card(Suit, Rank, glm::vec3, glm::vec3);
    glm::mat4 GetTransform();
};

Card::Card(Suit suit,
           Rank rank,
           glm::vec3 position,
           glm::vec3 scale){
    this->suit = suit;
    this->rank = rank;
    this->isFlipped = false;
    this->position = position;
    this->scale = scale;
}

Card::Card(){
    this->suit = SUIT_NONE;
    this->rank = RANK_NONE;
    this->isFlipped = false;
    this->position = glm::vec3(0.0f);
    this->scale = glm::vec3(1.0f);
}

glm::mat4 Card::GetTransform(){
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, this->position);
    transform = glm::scale(transform, this->scale);
    return transform;
}

#endif //CARD_H
