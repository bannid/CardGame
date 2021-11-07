#ifndef GAME_H
#define GAME_H

#include <irklang/irrKlang.h>
#include <cmath>
#include "..\input.h"
#include "..\card.h"
#include "..\screen.h"
#include "..\model.h"
#include "..\texture.h"
#include "..\textureManager.h"
#include "..\vao.h"
#include "..\shaderManager.h"

#define SOUND_FILE(fileName) "../assets/" #fileName

#define GAME_UPDATE_FUNCTION(name) void name(Game * game, float elapsedTime, float timeSinceStarting)
#define GAME_INIT_FUNCTION(name) void name(Game * game)
#define PLAY_SOUND_FUNCTION(name) void name(const char * filePath, bool loop)
typedef PLAY_SOUND_FUNCTION(PlaySoundCallback);
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

enum SoundType{
    CARD_CLICK,
    CARD_MATCH,
    GAME_WON
};

struct Level{
    nGame::Card * cards;
    int totalNumberOfCards;
    bool isWon = false;
    float elapsedTime = 0.0f;
    float totalTime = 3 * 60; // seconds
};

struct Game {
    GlobalInfo *                                                globalInfo;
    TextureAPI                                                  textureApi;
    Level *                                                     currentLevel;
    GameState                                                   state;
    MouseKeyIsDownCallback *                                    mouseKeyIsDownCallback;
    GetMousePositionCallback *                                  mousePositionCallback;
    Quad *                                                      front;
    Quad *                                                      back;
    Texture *                                                   textures;
    ShaderManager *                                             shaderManager;
    GetShaderShaderManagerCallback *                            getShaderShaderManagerCallback;
    AttachShaderCallback *                                      attachShaderCallback;
    VertexArrayObject *                                         vao;
    VertexArrayObject *                                         vaoBackground;
    TextureManager *                                            textureManager;
    GetTextureTextureManagerCallback *                          getTextureTextureManager;
    DrawQuadCallback *                                          drawQuadCallback;
    PlaySoundCallback *                                         playSoundCallback;
    DrawVaoCallback *                                           drawVaoCallback;
    AttachVaoCallback *                                         attachVaoCallback;
    ResetInputCallback *                                        resetInputCallback;
    LoadShaderShaderManagerCallback *                           loadShaderShaderManagerCallback;
    LoadShadersShaderManagerCallback *                          loadShadersShaderManagerCallback;
};


typedef GAME_UPDATE_FUNCTION(UpdateGameCallback);
typedef GAME_INIT_FUNCTION(InitGameCallback);
DLL_API GAME_UPDATE_FUNCTION(UpdateGame);
DLL_API GAME_INIT_FUNCTION(InitGame);
#endif //GAME_H
