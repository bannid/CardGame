#ifndef GAME_H
#define GAME_H

#include "input.h"
#include "card.h"
#include "screen.h"

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
    int totalNumberOfCards;
    bool isWon = false;
};

struct Game {
    GlobalInfo * globalInfo;
    Level * currentLevel;
    GameState state;
};

void UpdateGame(Game * game);
int GetNumberOfFlippedCards(Game * game);
int GetNumberOfMatchedCards(Game * game);

#endif //GAME_H
