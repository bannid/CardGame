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
        Button(Texture * texture, Game * game, ButtonPressCallback callback, glm::vec3 position);
    };
    void StartGame(Game * game);
    void QuitGame(Game * game);
    void PauseGame(Game * game);
    void ResumeGame(Game * game);
};

#endif //UI_H
