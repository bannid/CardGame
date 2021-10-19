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
    Card(Suit suit,
         Rank rank,
         glm::vec3 position,
         glm::vec3 scale);
    Card();
    bool shouldntBeFlipped = false;
};

#endif //CARD_H
