#ifndef UI_H
#define UI_H

#include "glmIncludes.h"
#include "game.h"
#include "texture.h"

namespace UI{
    typedef void(* ButtonPressCallback)(Game * game);
    struct Button{
        ButtonPressCallback callback;
        Texture * texture;
        Game * game;
        glm::vec3 position;
        Button(Texture * texture,
               Game * game,
               ButtonPressCallback callback,
               glm::vec3 position){
            this->callback = callback;
            this->texture = texture;
            this->game = game;
            this->position = position;
        }
    };
    void StartGame(Game * game){ game->state = GameState::PLAYING;}
    void QuitGame(Game * game){ game->state = GameState::EXITING;}
    void PauseGame(Game * game);
    void ResumeGame(Game * game);
};

#endif //UI_H
