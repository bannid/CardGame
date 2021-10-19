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

#define SIZE_OF_ARRAY(array, type) sizeof(array) / sizeof(type)

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
    if(!card->rotateAnimation.isActive){
        card->rotateAnimation.isActive = true;
        card->rotateAnimation.startingValue = card->rotateY;
        card->rotateAnimation.endingValue = card->rotateY + 180.0f;
        card->isFlipped = !card->isFlipped;
    }
}

int GetNumberOfFlippedCards(Card * cards, int numberOfCards){
    int number = 0;
    for(int i = 0; i<numberOfCards; i++){
        if(cards[i].isFlipped){
            number++;
        }
    }
    return number;
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
    //Enable back face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //Compile the shaders
    Shader shader("../shaders/vertexShader.vert", "../shaders/fragmentShader.vert");
    if (!shader.CompileAndLink()){
        glfwTerminate();
        return -1;
    }
    //Load the textures.
    Texture cardBack("../assets/card-back2.png", 4);
    if (!cardBack.Load()){
        glfwTerminate();
        return -1;
    }
    Texture textures[52];
    if(!LoadTextures(textures)){
        glfwTerminate();
        return -1;
    }
    
    float vertices[] = {
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //top left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //bottom left
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //bottom right
        
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //top left
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //bottom right
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f //top right
    };
    VertexArrayObject vao(vertices, SIZE_OF_ARRAY(vertices, float));
    shader.Attach(); 
    vao.Attach();
    glm::mat4 projectionMat =  glm::ortho(0.0f, globalOpenglX, globalOpenglY, 0.0f, -500.0f, 500.0f); 
    glm::mat4 camMat = glm::mat4(1.0f);
    shader.SetMat4("uProjectionMat", projectionMat);
    shader.SetMat4("uCamMat", camMat);
    double time = glfwGetTime();
    glfwSwapInterval(1);
    Key keyboardKeys[] = {
        Key(GLFW_KEY_LEFT),
        Key(GLFW_KEY_RIGHT),
        Key(GLFW_KEY_UP),
        Key(GLFW_KEY_DOWN)
    };
    
    Key mouseKeys[] = {
        Key(GLFW_MOUSE_BUTTON_LEFT)
    };
    
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
    for(int col = 0; col<numberOfColumns; col++){
        for(int row = 0; row<numberOfRows; row++){
            int index = row + col * numberOfRows;
            Card * card = cards + index;
            card->suit = (Suit)(rand() % 4);
            card->rank = (Rank)(rand() % 13);
            card->scale = glm::vec3(scale, scale, 1.0f);
            card->position = glm::vec3(scale + (col * scale * 2.0f) + (offsetX * col) + offsetX,
                                       scale + (row * scale * 2.0f) + (offsetY * row) + offsetY,
                                       0.0f);
        }
    }
    obj1.rotation = 180.0f;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    while(!glfwWindowShouldClose(window)){
        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - time;
        time = currentTime;
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //Imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //Update the input states.
        //Keyboard
        UpdateInputState(window, 
                         keyboardKeys,
                         SIZE_OF_ARRAY(keyboardKeys, Key),
                         glfwGetKey);
        //Mouse
        UpdateInputState(window,
                         mouseKeys,
                         SIZE_OF_ARRAY(mouseKeys, Key),
                         glfwGetMouseButton);
        
        //Rotate all the cards that shouldn't be flipped
        for(int i = 0; i<totalNumberOfCards; i++){
            Card * card = cards + i;
            if(card->shouldntBeFlipped && !card->rotateAnimation.isActive){
                ClickCard(card);
                card->shouldntBeFlipped = false;
            }
        }
        Card * cardClicked = NULL;
        //If the left mouse was clicked
        if(KeyWasReleased(GLFW_MOUSE_BUTTON_LEFT,
                          mouseKeys,
                          SIZE_OF_ARRAY(mouseKeys, Key))){
            double x, y;
            glfwGetCursorPos(window, &x, &y);
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
            if(totalFlippedCards > 2){
                //If there are more than two flipped cards,
                //we want to flip back all the cards that were flipped
                //previously.
                for(int i = 0; i<totalNumberOfCards; i++){
                    Card * card = cards + i;
                    if( card != cardClicked && card->isFlipped){
                        //If the previous card is still animating,
                        //set a flag to make sure the card will be set back.
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
                float weight = card->rotateAnimation.timeAccumulator / card->rotateAnimation.totalTime;
                AnimationRotate rot = card->rotateAnimation;
                card->rotateY = CosineInterpolation(rot.startingValue , rot.endingValue, weight);
                obj2.rotation = card->rotateY;
                obj1.rotation = card->rotateY + 180.0f;
            }
            textures[card->rank + card->suit * 13].Attach();
            obj1.Draw(&shader, &vao);
            cardBack.Attach();
            obj2.Draw(&shader, &vao);
        }
        ImGui::Begin("Hi there");
        ImGui::Text("Whatever");
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
    
    bool isInOnXAxis = coords.x >= leftX && coords.x <= rightX;
    bool isInOnYAxis = coords.y >= leftY && coords.y <= rightY;
    return isInOnXAxis && isInOnYAxis;
}