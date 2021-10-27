#ifndef GAME_H
#define GAME_H

#include "..\input.h"
#include "..\card.h"
#include "..\screen.h"
#define GAME_UPDATE_FUNCTION(name) void name(Game * game)
#define DLL_API extern "C" __declspec(dllexport)
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
    nGame::Card * cards;
    int totalNumberOfCards;
    bool isWon = false;
};

struct Game {
    GlobalInfo * globalInfo;
    Level * currentLevel;
    GameState state;
    MouseKeyIsDownCallback * mouseKeyIsDownCallback;
    GetMousePositionCallback * mousePositionCallback;
};
typedef GAME_UPDATE_FUNCTION(UpdateGameCallback);
DLL_API GAME_UPDATE_FUNCTION(UpdateGame);
int GetNumberOfFlippedCards(Game * game);
int GetNumberOfMatchedCards(Game * game);
#endif //GAME_H
