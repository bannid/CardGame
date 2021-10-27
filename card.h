#ifndef CARD_H
#define CARD_H

#include "cardTypes.h"
#include "glmIncludes.h"
#include "animators.h"
#include "screen.h"

namespace nGame {
    struct Card{
        Suit suit;
        Rank rank;
        glm::vec3 position;
        glm::vec3 scale;
        bool isFlipped;
        bool isMatched;
        Anim::AnimationRotate rotateAnimation;
        Card(Suit suit,
            Rank rank,
            glm::vec3 position,
            glm::vec3 scale);
        Card();
        bool shouldntBeFlipped = false;
        int clickCounter = 0;
    };

    void ClickCard(Card * card);
    bool CardWasClicked(Card * card, OpenglCoords coords);
};
#endif //CARD_H
