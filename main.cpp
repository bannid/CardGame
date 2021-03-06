
#include <iostream>
#include <map>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include <irklang/irrKlang.h>
#include "common.h"
#include "win32_fileapi.h"
#include "openglIncludes.h"
#include "glmIncludes.h"
#include "shader.h"
#include "vao.h"
#include "texture.h"
#include "model.h"
#include "cardTypes.h"
#include "animators.h"
#include "input.h"
#include "card.h"
#include "misc.h"
#include "game/game.h"
#include "ui.h"
#include "shaderManager.h"
#include "particle.h"
#include "imgui_plugin.h"
#include "screen.h"
#include "text.h"

GAME_UPDATE_FUNCTION(UpdateGameStub){}
GAME_INIT_FUNCTION(InitGameStub){}
struct GameCode {
    HINSTANCE lib;
    UpdateGameCallback * updateGame;
    InitGameCallback * initGame;
    bool isValid = false;
};

GlobalInfo globalInfo;
irrklang::ISoundEngine * globalSoundEngine;

bool            LoadTextures(Texture * textures);
void            FrameBufferSizeCallback(GLFWwindow * window, int width, int height);
bool            StartOpenglAndReturnWindow(GLFWwindow ** window);
bool            LoadGlad();
inline void     GlOptions();
inline void     InitalizeAllCards(nGame::Card * cards, int numberOfCols, int numberOfRows);
inline void     ShuffleCardsArray(nGame::Card * cards, int numberOfCards);
inline void     DrawCards(nGame::Card * cards, int numberOfCards, Quad * objFront, Quad * objBack);
inline void     StartGame(Game * game);

PLAY_SOUND_FUNCTION(PlaySoundCardGame){
    globalSoundEngine->play2D(filePath, loop);
}
PLAY_SOUND_FUNCTION(PlaySoundStub){

}
void LoadGameDLL(GameCode * gameCode){
    if (gameCode->isValid) FreeLibrary(gameCode->lib);
    while (CopyFileA("game.dll", "gameTemp.dll", FALSE) == 0) {
        
    }
    gameCode->lib = LoadLibraryA("gameTemp.dll");
    if(gameCode->lib != NULL){
        gameCode->updateGame = (UpdateGameCallback*) GetProcAddress(gameCode->lib, "UpdateGame");
        gameCode->initGame = (InitGameCallback*) GetProcAddress(gameCode->lib, "InitGame");
        if(gameCode->updateGame != NULL && gameCode->initGame != NULL){
            gameCode->isValid = true;
        }
        else{
            gameCode->updateGame = UpdateGameStub;
            gameCode->initGame = InitGameStub;
            FreeLibrary(gameCode->lib);
            gameCode->isValid = false;
        }
    }
}

int CALLBACK WinMain(HINSTANCE instance,
					 HINSTANCE prevInstance,
					 LPSTR commandLine,
					 int showCode) {
    globalInfo.aspectRatio = 1.0f;
    globalInfo.rescale = false;
    globalInfo.windowHeight = 800.0f;
    globalInfo.windowWidth = 800.0f;
    globalInfo.openglWidth = 400.0f;
    globalInfo.openglHeight = 400.0f;
    GLFWwindow * window;
    if(!StartOpenglAndReturnWindow(&window)){
        return -1;
    }
    if(!LoadGlad()){
        glfwTerminate();
        return -1;
    }
    //Some GL options like Blending and face culling
    GlOptions();
    globalSoundEngine= irrklang::createIrrKlangDevice();
    if(globalSoundEngine == NULL){
        //do something;
    }

    //Compile the shaders
    Shader backgroundShader;
    Shader circleShader;
    Shader shader;
    Shader cardBackShader;
    Shader loadingCircleShader;
    Shader textShader;
    ShaderManager sm;
    sm.LoadShaders({
                       { "BackGroundShader", "../shaders/backgroundShader.vert", "../shaders/backgroundShader.frag" },
                       { "CircleShader", "../shaders/backgroundShader.vert", "../shaders/circles.frag" },
                       { "GameShader", "../shaders/vertexShader.vert", "../shaders/fragmentShader.vert"},
                       { "CardBackShader", "../shaders/vertexShader.vert", "../shaders/cardBackShader.frag"},
                       { "LoadingCircleShader", "../shaders/vertexShader.vert", "../shaders/loadingCircleShader.frag"},
                       { "TextShader", "../shaders/vertexShader.vert", "../shaders/textShader.frag"},
                   });
    sm.GetShader("GameShader", &shader);
    sm.GetShader("CircleShader", &circleShader);
    sm.GetShader("BackGroundShader", &backgroundShader);
    sm.GetShader("CardBackShader", &cardBackShader);
    sm.GetShader("LoadingCircleShader", &loadingCircleShader);
    sm.GetShader("TextShader", &textShader);
    TextureManager texManager;
    LoadTextureTextureManager(&texManager, "../assets/card-back2.png", "CardBack", 4);
    LoadTextureTextureManager(&texManager, "../assets/UI/Start.png","StartButton", 4);
    LoadTextureTextureManager(&texManager, "../assets/UI/Quit.png", "QuitButton", 4);
    //Load the textures.
    Texture cardBack;
    LoadTexture(&cardBack, "../assets/card-back2.png", "CardBack", 4, true);
    Texture startButton;
    LoadTexture(&startButton, "../assets/UI/Start.png","StartButton", 4, true);
    Texture quitButton;
    LoadTexture(&quitButton, "../assets/UI/Quit.png", "QuitButton", 4, true);
    Texture arialFontTexture;
    LoadTexture(&arialFontTexture, "../assets/arial.png", "ArialFontAtlas", 4, false);
    if (!cardBack.loaded || !startButton.loaded || !quitButton.loaded){
        glfwTerminate();
        if(globalSoundEngine != NULL)globalSoundEngine->drop();
        return -1;
    }
    
    Texture textures[52];
    if(!LoadTextures(textures)){
        glfwTerminate();
        if(globalSoundEngine != NULL)globalSoundEngine->drop();
        return -1;
    }
    
    //Vertices for the cards and UI
    float vertices[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,//top left
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,//bottom left
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,// top right
        
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,//bottom left
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,//bottom right
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f//top right 
    };
    VertexArrayObject vao;
    LoadVao(&vao, vertices, CNT_ARR(vertices), false);
    VertexArrayObject vaoText;
    LoadVao(&vaoText, NULL, CNT_ARR(vertices), true);
    Quad text;
    CharacterSet arials;
    arials.fontAtlasTexture = &arialFontTexture;
    arials.quad = &text;
    arials.shader = &textShader;
    arials.vao = &vaoText;

    LoadFonts("../assets/arial.fnt", &arials);
    glm::mat4 projectionMat =  glm::ortho(0.0f, globalInfo.openglWidth, 0.0f, globalInfo.openglHeight, -500.0f, 500.0f);
    glm::mat4 camMat = glm::mat4(1.0f);
    SetMat4Shader(&shader, "uProjectionMat", projectionMat);
    SetMat4Shader(&shader, "uCamMat", camMat);

    SetMat4Shader(&cardBackShader, "uProjectionMat", projectionMat);
    SetMat4Shader(&cardBackShader, "uCamMat", camMat);
    
    SetMat4Shader(&loadingCircleShader, "uProjectionMat", projectionMat);
    SetMat4Shader(&loadingCircleShader, "uCamMat", camMat);
    
    SetMat4Shader(&textShader, "uProjectionMat", projectionMat);
    SetMat4Shader(&textShader, "uCamMat", camMat);
    

    double time = glfwGetTime();
    double timeAtStart = glfwGetTime();
    glfwSwapInterval(1);
    SetVec3Shader(&backgroundShader, "resolution", glm::vec3(800.0f, 800.0f, 1.0f));
    SetVec3Shader(&circleShader, "resolution", glm::vec3(800.0f, 800.0f, 1.0f));
    Quad obj1;
    Quad obj2;
    Quad buttons;
    //Setup the cards.
    const int numberOfColumns = 4;
    const int numberOfRows = 6;
    const int totalNumberOfCards = numberOfColumns * numberOfRows;
    nGame::Card cards[numberOfColumns * numberOfRows];
    InitalizeAllCards(cards, numberOfColumns, numberOfRows);
    ShuffleCardsArray(cards, totalNumberOfCards);
    Level level1;
    level1.cards = cards;
    level1.totalNumberOfCards = totalNumberOfCards;
    //Pass the apis to the game dll.
    Game game;
    game.currentLevel = &level1;
    game.globalInfo = &globalInfo;
    game.front = &obj1;
    game.back = &obj2;
    game.textures = textures;
    game.textureManager = &texManager;
    game.vao = &vao;
    game.shaderManager = &sm;
    game.getShaderShaderManagerCallback = GetShaderShaderManager;
    game.attachShaderCallback = AttachShader;
    game.state = GameState::STARTMENU;
    game.mouseKeyIsDownCallback = Input::MouseKeyWasReleased;
    game.mousePositionCallback = Input::GetMousePositions;
    game.resetInputCallback = Input::ResetState;
    game.textureApi.attachCallback = AttachTexture;
    game.textureApi.loadCallback = LoadTexture;
    game.drawQuadCallback = DrawQuad;
    game.getTextureTextureManager = GetTextureTextureManager;
    game.playSoundCallback = globalSoundEngine == NULL ? PlaySoundStub : PlaySoundCardGame;
    game.loadShaderShaderManagerCallback = LoadShaderShaderManager;
    game.loadShadersShaderManagerCallback = LoadShadersShaderManager;
    game.renderTextCallback = RenderText;
    game.characterSet = &arials;
    IMGUI_INIT(window);
    Input::Key keyboardKeys[] = {
        Input::Key(GLFW_KEY_LEFT),
        Input::Key(GLFW_KEY_RIGHT),
        Input::Key(GLFW_KEY_UP),
        Input::Key(GLFW_KEY_DOWN)
    };
    
    Input::Key mouseKeys[] = {
        Input::Key(GLFW_MOUSE_BUTTON_LEFT)
    };
    //Initialize the input system.
    Input::Initialize(window, keyboardKeys, mouseKeys);
    float offsetFromTop = 25.0f;
    glm::vec3 buttonsPosition = glm::vec3(200.0f, 180.0f, 1.0f);
    UI::Button startUpButtons[] = {
        UI::Button(&quitButton, &game, UI::QuitGame, buttonsPosition),
        UI::Button(&startButton, &game, StartGame, buttonsPosition)
    };
    GameCode gameCode;
    LoadGameDLL(&gameCode);
    gameCode.initGame(&game);
    while(!glfwWindowShouldClose(window) && game.state != GameState::EXITED){
        float timeSinceStarting = glfwGetTime() - timeAtStart;
        glClearColor(.0f, .0f, .0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Draw the background
        SetFloatShader(&backgroundShader, "iTime", timeSinceStarting);
        SetFloatShader(&shader, "iTime", timeSinceStarting);
        SetFloatShader(&loadingCircleShader, "iTime", timeSinceStarting);
        SetFloatShader(&loadingCircleShader, "timeLeftNormalized", game.currentLevel->elapsedTime / game.currentLevel->totalTime);
        SetFloatShader(&circleShader, "iTime", timeSinceStarting);
        AttachShader(&backgroundShader);
        DrawVao(&vao);
        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - time;
        time = currentTime;
        IMGUI_NEW_FRAME();
        Input::UpdateInputState();
        switch(game.state){
            case GameState::STARTMENU:{
                AttachShader(&circleShader);
                DrawVao(&vao);
                buttons.scale = glm::vec3(20.0f, 10.0f, 1.0f);
                for(int i = 0; i<CNT_ARR(startUpButtons); i++){
                    AttachTexture(startUpButtons[i].texture);
                    buttons.position = startUpButtons[i].position + glm::vec3(0.0f, offsetFromTop * i, 1.0f);
                    DrawQuad(&buttons, &shader, &vao);
                }
                if(Input::MouseKeyWasReleased(GLFW_MOUSE_BUTTON_LEFT)){
                    double x, y;
                    Input::GetMousePositions(&x, &y);
                    OpenglCoords coordsOpengl = ScreenToOpenglCoords(x, y, &globalInfo);
                    std::cout<<x<<","<<y<<std::endl;
                    for(int i = 0; i<CNT_ARR(startUpButtons); i++){
                        UI::Button * button = startUpButtons + i;
                        float leftX = button->position.x - buttons.scale.x;
                        float leftY = button->position.y + (offsetFromTop * i) - buttons.scale.y;
                        float rightX = button->position.x + buttons.scale.x;
                        float rightY = button->position.y + (offsetFromTop * i) + buttons.scale.y;
                        if(RectangleWasClicked(leftX, leftY, rightX, rightY, coordsOpengl)){
                            button->callback(&game);
                            Input::ResetState();
                        }
                    }
                }
                break;
            }
            case GameState::EXITING: {
                game.state = GameState::EXITED;
                break;
            }
        }
        gameCode.updateGame(&game, elapsedTime, timeSinceStarting);
        IMGUI_FUNCTION(ImGui::Begin("General info"));
        IMGUI_FUNCTION(ImGui::Text("LF: %0f ms", elapsedTime * 1000.0f));
        IMGUI_FUNCTION(ImGui::Text("FPS: %0f", 1.0f / elapsedTime));
        if(game.currentLevel->elapsedTime < 60.0f){
            IMGUI_FUNCTION(ImGui::Text("Elapsed time: %d seconds", (int)game.currentLevel->elapsedTime));
        }
        else{
            float minutes = game.currentLevel->elapsedTime / 60.0f;
            int seconds = (int)(game.currentLevel->elapsedTime)%60;
            IMGUI_FUNCTION(ImGui::Text("Elapsed time: %d min %d seconds", (int)minutes, seconds));
        }
        if(DEBUG_IF(ImGui::Button("Reload"))){
            LoadGameDLL(&gameCode);
        }
        //ImGui::ShowDemoWindow();
        IMGUI_FUNCTION(ImGui::End());
        IMGUI_FUNCTION(ImGui::Begin("Textures"));
        for(int i = 0; i<texManager.numberOfTextures; i++){
            IMGUI_FUNCTION(ImGui::Text(texManager.textures[i].textureName.c_str()));
        }
        IMGUI_FUNCTION(ImGui::End());
        IMGUI_FUNCTION(ImGui::Begin("Shaders"));
        for(int i = 0; i<sm.shaders.size(); i++){
            IMGUI_FUNCTION(ImGui::Text(sm.shaders[i].name.c_str()));
        }
        char shaderPath[128] = "Hello there";
        IMGUI_FUNCTION(ImGui::InputText("Shader path", shaderPath, CNT_ARR(shaderPath)));
        IMGUI_FUNCTION(ImGui::End());
        IMGUI_RENDER();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    IMGUI_EXIT();
    
    glfwTerminate();
    FreeLibrary(gameCode.lib);
    if(globalSoundEngine != NULL)globalSoundEngine->drop();
    return 0;
}

bool LoadTextures(Texture * textures){
    std::string stringOne("card-");
    std::string stringTwo[4] = {
        std::string("diamonds-"),
        std::string("clubs-"),
        std::string("spades-"),
        std::string("hearts-")
    };
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            std::string temp = std::to_string(j + 1);
            std::string filePath = "../assets/pfc/pc/" + 
                stringOne +
                stringTwo[i] +
                temp + ".png";
            int index = j + i * 13;
            Texture * currentTexture = textures + index;
            if(!LoadTexture(currentTexture,filePath.c_str(), "PlayingCard", 4, true)){
                return false;
            }
            
        }
    }
    return true;
}

void StartGame(Game * game){
    game->state = GameState::PLAYING;
    game->currentLevel->isWon = false;
    game->currentLevel->elapsedTime = 0.0f;
    game->currentLevel->gameOverScreenTimeElapsed = 0.0f;
    InitalizeAllCards(game->currentLevel->cards, 4, 6);
    ShuffleCardsArray(game->currentLevel->cards, 6 * 4);
    globalSoundEngine->stopAllSounds();
}

void FrameBufferSizeCallback(GLFWwindow * window, int width, int height){
    globalInfo.aspectRatio = (float)width / height;
    globalInfo.windowHeight = height;
    globalInfo.windowWidth = width;
    globalInfo.rescale = true;
    glViewport(0, 0, width, height);
}

bool StartOpenglAndReturnWindow(GLFWwindow ** window){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    *window = glfwCreateWindow(globalInfo.windowWidth, globalInfo.windowHeight, "CardsGame", NULL, NULL);
    if ( *window == NULL ) {
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, FrameBufferSizeCallback);
    return true;
}

bool LoadGlad(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return false;
    }
    return true;
}

inline void GlOptions(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

inline void ShuffleCardsArray(nGame::Card * cards, int numberOfCards){
    for(int i = 0; i<numberOfCards; i++){
        nGame::Card * card = cards + i;
        nGame::Card tempCard = *card;
        int randomIndex = rand() % numberOfCards;
        nGame::Card * destinationCard = cards + randomIndex;
        card->suit = destinationCard->suit;
        card->rank = destinationCard->rank;
        destinationCard->suit = tempCard.suit;
        destinationCard->rank = tempCard.rank;
    }
}

inline void InitalizeAllCards(nGame::Card * cards, int numberOfCols, int numberOfRows){
    const float scale = 25.0f;
    const float offsetX = 10.0f;
    const float offsetY = 10.0f;
    const float firstOffsetY = 20.0f;
    const int totalNumberOfCards = numberOfCols * numberOfRows;
    //Initialize all the cards.
    for(int col = 0; col<numberOfCols; col++){
        for(int row = 0; row<numberOfRows; row++){
            int index = row + col * numberOfRows;
            nGame::Card * card = cards + index;
            float scaleX = scale / globalInfo.aspectRatio;
            card->scale = glm::vec3(scaleX, scale, 1.0f);
            card->position = glm::vec3(scaleX + (col * scaleX * 2.0f) + (offsetX * col) + offsetX,
                                       scale + (row * scale * 2.0f) + (offsetY * row) + firstOffsetY,
                                       0.0f);
            card->isFlipped = false;
            card->isGone = false;
            card->isMatched = false;
            card->rotateAnimation.currentValue = 0.0f;
            card->rotateAnimation.isActive = false;
            card->clickCounter = 0.0f;
        }
    }
    //Assign two sets of random suits and ranks to the cards.
    for(int i = 0; i<totalNumberOfCards / 2; i++){
        nGame::Card * card = cards + i;
        nGame::Card * cardOtherHalf = cards + i + totalNumberOfCards / 2;
        Suit suit = (Suit)(rand() % 4);
        Rank rank = (Rank)(rand() % 13);
        card->suit = suit;
        card->rank = rank;
        cardOtherHalf->suit = suit;
        cardOtherHalf->rank = rank;
    }
}