#ifndef GAME_H
#define GAME_H

#include "..\input.h"
#include "..\card.h"
#include "..\screen.h"
#include "..\model.h"
#include "..\texture.h"
#include "..\textureManager.h"
#include "..\vao.h"

#define GAME_UPDATE_FUNCTION(name) void name(Game * game, float elapsedTime)
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
    TextureAPI textureApi;
    Level * currentLevel;
    GameState state;
    MouseKeyIsDownCallback * mouseKeyIsDownCallback;
    GetMousePositionCallback * mousePositionCallback;
    Object3D * front;
    Object3D * back;
    Texture * textures;
    Shader * shader;
    VertexArrayObject * vao;
    TextureManager * textureManager;
    GetTextureTextureManagerCallback * getTextureTextureManager;
    DrawObject3DCallback * drawObject3D;
};


typedef GAME_UPDATE_FUNCTION(UpdateGameCallback);
DLL_API GAME_UPDATE_FUNCTION(UpdateGame);
#endif //GAME_H
