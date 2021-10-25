#include "ui.h"

 UI::Button::Button(Texture * texture, Game * game, ButtonPressCallback callback, glm::vec3 position){
    this->callback = callback;
    this->texture = texture;
    this->game = game;
    this->position = position;
}

void UI::StartGame(Game * game) { game->state = GameState::PLAYING; }
void UI::QuitGame(Game * game) { game->state = GameState::EXITING; }
void UI::PauseGame(Game * game) { game->state = GameState::PAUSED; }
void UI::ResumeGame(Game * game) {game->state = GameState::PLAYING; }