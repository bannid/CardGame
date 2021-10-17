#include "card.h"

Card::Card(CardSuit suit,
           CardRank cardRank,
           glm::vec3 position,
           glm::vec3 scale){
    this->SetSuit(suit);
    this->SetRank(cardRank);
    this->SetPosition(position);
    this->SetScale(scale);
    this->flipped = false;
}

void Card::Initialize(){
    this->front.Scale(this->scale);
    this->back.Scale(glm::vec3(50.0f, 50.0f, 1.0f));
    //this->back.Rotate(180.0f);
    this->back.Translate(this->position);
    this->front.Translate(this->position);
}

Card::Card(){
    this->position = glm::vec3(0.0f);
    this->scale = glm::vec3(50.0f);
}

void Card::SetSuit(CardSuit suit){
    this->suit = suit;
}

void Card::SetRank(CardRank rank){
    this->cardRank = rank;
}

void Card::SetPosition(glm::vec3 pos){
    this->position = pos;
    this->front.Translate(this->position);
    this->back.Translate(this->position);
}

void Card::SetScale(glm::vec3 scale){
    this->scale = scale;
    this->front.Scale(this->scale);
    this->back.Scale(this->scale);
}

void Card::Rotate(float angle){
    this->front.Rotate(angle);
    this->back.Rotate(angle);
}