#ifndef GAME_H
#define GAME_H

#include "input.h"
#include "card.h"
enum GameState{
    STARTUP,
    STARTMENU,
    LOADING_LEVEL,
    PLAYING,
    PAUSED,
    GAME_OVER,
    EXITING,
    EXITED
};

struct Level{
    Card * cards;
    bool isWon = false;
};

struct Game {
    Level * currentLevel;
    GameState state;
};

#endif //GAME_H
