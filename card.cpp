#include "card.h"

Card::Card(Suit suit,
           Rank rank,
           glm::vec3 position,
           glm::vec3 scale){
    this->suit = suit;
    this->rank = rank;
    this->isFlipped = false;
    this->isMatched = false;
    this->position = position;
    this->scale = scale;
}

Card::Card(){
    this->suit = SUIT_NONE;
    this->rank = RANK_NONE;
    this->isFlipped = false;
    this->isMatched = false;
    this->position = glm::vec3(0.0f);
    this->scale = glm::vec3(1.0f);
}