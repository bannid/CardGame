#include "card.h"

Card::Card(CardSuit suit, CardRank cardRank, glm::vec3 position){
    this->SetSuit(suit);
    this->SetRank(cardRank);
    this->SetPosition(position);
    this->flipped = false;
}

void Card::Initialize(){
    this->front.Scale(glm::vec3(50.0f, 50.0f, 1.0f));
    this->back.Scale(glm::vec3(50.0f, 50.0f, 1.0f));
    this->back.Rotate(180.0f);
}

Card::Card(){}

void Card::SetSuit(CardSuit suit){
    this->suit = suit;
}

void Card::SetRank(CardRank rank){
    this->cardRank = rank;
}

void Card::SetPosition(glm::vec3 pos){
    this->position = pos;
}

void Card::Draw(Shader * shader, VertexArrayObject * vao){
    this->front.Draw(shader, vao);
}