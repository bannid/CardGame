#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
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
#include "game.h"
#include "ui.h"

#define ARR_SIZ(array, type) sizeof(array) / sizeof(type)

#define NUMBER_OF_COLUMNS 4
#define NUMBER_OF_ROWS 4

//Globals
static float globalAspectRatio = 1.0f;
static bool globalRescale = false;

static int globalWindowHeight = 800;
static int globalWindowWidth = 800;
static float globalOpenglX = 400.0f;
static float globalOpenglY = 400.0f;

struct OpenglCoords{
    float x;
    float y;
};
//Textures
bool LoadTextures(Texture * textures);
OpenglCoords ScreenToOpenglCoords(float x, float y);
//Click handlers
bool CardWasClicked(Card * card, OpenglCoords coords);
bool RectangleWasClicked(float leftX, 
                         float leftY, 
                         float rightX, 
                         float rightY, 
                         OpenglCoords coords);
//Animations 
void UpdateAnimationState(Card * card, float elapsedTime);
void FrameBufferSizeCallback(GLFWwindow * window, int width, int height){
    globalAspectRatio = (float)width / height;
    globalWindowHeight = height;
    globalWindowWidth = width;
    globalRescale = true;
    glViewport(0, 0, width, height);
}

bool StartOpenglAndReturnWindow(GLFWwindow ** window){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow * windowTemp = glfwCreateWindow(globalWindowWidth, globalWindowHeight, "CardsGame", NULL, NULL);
    if ( windowTemp == NULL ) {
        return false;
    }
    *window = windowTemp;
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

void ClickCard(Card * card){
    if(!card->rotateAnimation.isActive && !card->isMatched){
        card->rotateAnimation.isActive = true;
        card->rotateAnimation.startingValue = card->rotateY;
        card->rotateAnimation.endingValue = card->rotateY + 180.0f;
        card->isFlipped = !card->isFlipped;
    }
    else{
        card->clickCounter++;
    }
}

int GetNumberOfFlippedCards(Card * cards, int numberOfCards){
    int number = 0;
    for(int i = 0; i<numberOfCards; i++){
        if(cards[i].isFlipped && !cards[i].isMatched){
            number++;
        }
    }
    return number;
}

int GetNumberOfMatchedCards(Card * cards, int numberOfCards){
    int number = 0;
    for(int i = 0; i<numberOfCards; i++){
        if(cards[i].isMatched){
            number++;
        }
    }
    return number;
}

inline void UpdateGame(Game * game, int totalNumberOfCards){
    Card * cards = game->currentLevel->cards;
    //Rotate all the cards that shouldn't be flipped
    for(int i = 0; i<totalNumberOfCards; i++){
        Card * card = cards + i;
        if(card->shouldntBeFlipped && !card->rotateAnimation.isActive){
            ClickCard(card);
            card->shouldntBeFlipped = false;
        }
    }
    if(game->state != PLAYING) return;
    Card * cardClicked = NULL;
    //If the left mouse button was clicked
    if(Input::MouseKeyWasReleased(GLFW_MOUSE_BUTTON_LEFT)){
        double x, y;
        Input::GetMousePositions(&x, &y);
        OpenglCoords coordsOpengl = ScreenToOpenglCoords(x, y);
        for(int i = 0; i<totalNumberOfCards; i++){
            Card * card = cards + i;
            if(CardWasClicked(card, coordsOpengl)){
                ClickCard(card);
                cardClicked = card;
            }
        }
    }
    //We check if the card clicked was flipped.
    if(cardClicked != NULL && cardClicked->isFlipped){
        //We check if there are more than 2 cards flipped.
        int totalFlippedCards = GetNumberOfFlippedCards(cards,
                                                        totalNumberOfCards);
        if(totalFlippedCards == 2){
            Card * firstCard = NULL;
            Card * secondCard = NULL;
            int counter = 0;
            for(int i = 0; i<totalNumberOfCards; i++){
                Card * card = cards + i;
                if(card->isFlipped && !card->isMatched){
                    if(counter == 0){
                        firstCard = card;
                        counter++;
                    }
                    else {
                        secondCard = card;
                        counter++;
                    }
                    
                }
            }
            if(firstCard->suit == secondCard->suit && 
               firstCard->rank == secondCard->rank){
                firstCard->isMatched = true;
                secondCard->isMatched = true;
            }
        }
        else if(totalFlippedCards > 2){
            //If there are more than two flipped cards,
            //we want to flip back all the cards that were flipped
            //previously.
            for(int i = 0; i<totalNumberOfCards; i++){
                Card * card = cards + i;
                if( card != cardClicked && card->isFlipped && !card->isMatched){
                    //If the previous card is still animating,
                    //set a flag to make sure the card will be flipped  back.
                    if(card->rotateAnimation.isActive){
                        card->shouldntBeFlipped = true;
                    }
                    //Otherwise just click the card
                    else{
                        ClickCard(card);
                    }
                }
            }
        }
    }
}

inline void UpdateAnimation(Card * card, Object3D * obj1, Object3D * obj2){
    float weight = card->rotateAnimation.timeAccumulator / card->rotateAnimation.totalTime;
    AnimationRotate rot = card->rotateAnimation;
    card->rotateY = CosineInterpolation(rot.startingValue , rot.endingValue, weight);
    obj2->rotation = card->rotateY;
    obj1->rotation = card->rotateY + 180.0f;
}

inline void ShuffleCardsArray(Card * cards, int numberOfCards){
    for(int i = 0; i<numberOfCards; i++){
        Card * card = cards + i;
        Card tempCard = *card;
        int randomIndex = rand() % numberOfCards;
        Card * destinationCard = cards + randomIndex;
        card->suit = destinationCard->suit;
        card->rank = destinationCard->rank;
        destinationCard->suit = tempCard.suit;
        destinationCard->rank = tempCard.rank;
    }
}

int CALLBACK WinMain(HINSTANCE instance,
					 HINSTANCE prevInstance,
					 LPSTR commandLine,
					 int showCode) {
    GLFWwindow * window;
    if(!StartOpenglAndReturnWindow(&window)){
        return -1;
    }
    if(!LoadGlad()){
        glfwTerminate();
        return -1;
    }
      // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Enable back face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //Compile the shaders
    Shader backgroundShader("../shaders/backgroundShader.vert",
                            "../shaders/backgroundShader.frag");
    Shader shader("../shaders/vertexShader.vert",
                  "../shaders/fragmentShader.vert");
    if(!backgroundShader.CompileAndLink()){
        glfwTerminate();
        return -1;
    }
    if(!shader.CompileAndLink()){
        glfwTerminate();
        return -1;
    }
    //Load the textures.
    Texture cardBack("../assets/card-back2.png", 4);
    Texture startButton("../assets/UI/Start.png", 4);
    Texture quitButton("../assets/UI/Quit.png", 4);
    if (!cardBack.Load() || !startButton.Load() || !quitButton.Load()){
        glfwTerminate();
        return -1;
    }
    Texture textures[52];
    if(!LoadTextures(textures)){
        glfwTerminate();
        return -1;
    }
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float verticesBackGround[] = {
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,//top left
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f,//bottom left
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f,// top right
        
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f,//bottom left
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f,//bottom right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f//top right 
    };
    float vertices[] = {
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //top left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //bottom left
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //bottom right
        
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //top left
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //bottom right
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f //top right
    };
    
    VertexArrayObject vao(vertices, ARR_SIZ(vertices, float));
    VertexArrayObject vaoBackground(verticesBackGround, ARR_SIZ(verticesBackGround, float));
    glm::mat4 projectionMat =  glm::ortho(0.0f, globalOpenglX, globalOpenglY, 0.0f, -500.0f, 500.0f); 
    glm::mat4 camMat = glm::mat4(1.0f);
    shader.SetMat4("uProjectionMat", projectionMat);
    shader.SetMat4("uCamMat", camMat);
    double time = glfwGetTime();
    double timeAtStart = glfwGetTime();
    glfwSwapInterval(1);
    backgroundShader.SetVec3("resolution", glm::vec3(800.0f, 800.0f, 1.0f));
    Object3D obj1;
    Object3D obj2;
    const float scale = 30.0f;
    const float offsetX = 10.0f;
    const float offsetY = 10.0f;
    //Setup the cards.
    const int numberOfColumns = 4;
    const int numberOfRows = 5;
    const int totalNumberOfCards = numberOfColumns * numberOfRows;
    Card cards[numberOfColumns * numberOfRows];
    //Initialize all the cards.
    for(int col = 0; col<numberOfColumns; col++){
        for(int row = 0; row<numberOfRows; row++){
            int index = row + col * numberOfRows;
            Card * card = cards + index;
            float scaleX = scale / globalAspectRatio;
            card->scale = glm::vec3(scaleX, scale, 1.0f);
            card->position = glm::vec3(scaleX + (col * scaleX * 2.0f) + (offsetX * col) + offsetX,
                                       scale + (row * scale * 2.0f) + (offsetY * row) + offsetY,
                                       0.0f);
        }
    }
    //Assign two sets of random suits and ranks to the cards.
    for(int i = 0; i<totalNumberOfCards / 2; i++){
        Card * card = cards + i;
        Card * cardOtherHalf = cards + i + totalNumberOfCards / 2;
        Suit suit = (Suit)(rand() % 4);
        Rank rank = (Rank)(rand() % 13);
        card->suit = suit;
        card->rank = rank;
        cardOtherHalf->suit = suit;
        cardOtherHalf->rank = rank;
    }

    ShuffleCardsArray(cards, totalNumberOfCards);
    Level level1;
    level1.cards = cards;
    
    Game game;
    game.currentLevel = &level1;
    game.state = GameState::STARTMENU;
    //Imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    //Initialize the input system.
    Input::Initialize(window);
    float offsetFromTop = 25.0f;
    glm::vec3 buttonsPosition = glm::vec3(200.0f, 180.0f, 1.0f);
    UI::Button startUpButtons[] = {
        UI::Button(&startButton, &game, UI::StartGame, buttonsPosition),
        UI::Button(&quitButton, &game, UI::QuitGame, buttonsPosition)
    };
    while(!glfwWindowShouldClose(window) && game.state != EXITED){
        glClearColor(1.0f, .2f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        backgroundShader.SetFloat("iTime", glfwGetTime() - timeAtStart);
        backgroundShader.Attach();
        vaoBackground.Draw();
        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - time;
        time = currentTime;
        //Imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        Input::UpdateInputState();
        switch(game.state){
            case GameState::PLAYING:{
                //Main loop that draws all the cards.
                for(int i = 0; i < totalNumberOfCards; i++){
                    Card * card = cards + i;
                    UpdateAnimationState(card, elapsedTime);
                    obj1.scale = card->scale;
                    obj2.scale = card->scale;
                    obj1.position = card->position;
                    obj2.position = card->position;
                    obj1.rotation = card->rotateY + 180.0f;
                    obj2.rotation = card->rotateY;
                    if(card->rotateAnimation.isActive){
                        UpdateAnimation(card, &obj1, &obj2);
                    }
                    textures[card->rank + card->suit * 13].Attach();
                    obj1.Draw(&shader, &vao);
                    cardBack.Attach();
                    obj2.Draw(&shader, &vao);
                }
                break;
            }
            case GameState::STARTMENU:{
                obj2.scale = glm::vec3(20.0f, 10.0f, 1.0f);
                for(int i = 0; i<ARR_SIZ(startUpButtons, UI::Button); i++){
                    startUpButtons[i].texture->Attach();
                    obj2.position = startUpButtons[i].position + glm::vec3(0.0f, offsetFromTop * i, 1.0f);
                    obj2.Draw(&shader, &vao);
                }
                if(Input::MouseKeyWasReleased(GLFW_MOUSE_BUTTON_LEFT)){
                    double x, y;
                    Input::GetMousePositions(&x, &y);
                    OpenglCoords coordsOpengl = ScreenToOpenglCoords(x, y);
                    for(int i = 0; i<ARR_SIZ(startUpButtons, UI::Button); i++){
                        UI::Button * button = startUpButtons + i;
                        float leftX = button->position.x - obj2.scale.x;
                        float leftY = button->position.y + (offsetFromTop * i) - obj2.scale.y;
                        float rightX = button->position.x + obj2.scale.x;
                        float rightY = button->position.y + (offsetFromTop * i) + obj2.scale.y;
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
        UpdateGame(&game, totalNumberOfCards);
        ImGui::Begin("General info");
        ImGui::Text("LF: %0f ms", elapsedTime * 1000.0f);
        ImGui::Text("FPS: %0f", 1.0f / elapsedTime);
        float f0;
        ImGui::InputFloat("input float", &offsetFromTop, 0.01f, 1.0f, "%.3f");
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();
    return 0;
}

void UpdateAnimationState(Card * card, float elapsedTime){
    if(card->rotateAnimation.isActive){
        if(card->rotateAnimation.timeAccumulator >= card->rotateAnimation.totalTime){
            card->rotateAnimation.isActive = false;
            card->rotateAnimation.timeAccumulator = 0.0f;
        }
        else{
            card->rotateAnimation.timeAccumulator += elapsedTime;
        }
        card->rotateAnimation.timeAccumulator = card->rotateAnimation.timeAccumulator > card->rotateAnimation.totalTime ? card->rotateAnimation.totalTime : card->rotateAnimation.timeAccumulator;
    }
    
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
            if(!currentTexture->Load(filePath.c_str(), 4)){
                return false;
            }
            
        }
    }
    return true;
}

OpenglCoords ScreenToOpenglCoords(float x, float y){
    OpenglCoords local;
    local.x = (x / globalWindowWidth) * globalOpenglX;
    local.y = (y / globalWindowHeight) * globalOpenglY;
    return local;
} 

bool CardWasClicked(Card * card, OpenglCoords coords){
    float leftX = card->position.x - card->scale.x;
    float leftY = card->position.y - card->scale.y;
    float rightX = card->position.x + card->scale.x;
    float rightY = card->position.y + card->scale.y;
    return RectangleWasClicked(leftX, leftY,
                               rightX, rightY,
                               coords);
}

bool RectangleWasClicked(float leftX, 
                         float leftY, 
                         float rightX, 
                         float rightY, 
                         OpenglCoords coords){
    bool isInOnXAxis = coords.x >= leftX && coords.x <= rightX;
    bool isInOnYAxis = coords.y >= leftY && coords.y <= rightY;
    return isInOnXAxis && isInOnYAxis;
}