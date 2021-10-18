#include <iostream>
#include <windows.h>
#include <math.h>
#include <string>
#include "win32_fileapi.h"
#include "openglIncludes.h"
#include "glmIncludes.h"
#include "shader.h"
#include "vao.h"
#include "texture.h"
#include "model.h"
#include "cardTypes.h"
#include "animators.h"

#define SIZE_OF_ARRAY(array, type) sizeof(array) / sizeof(type)

#define NUMBER_OF_COLUMNS 4
#define NUMBER_OF_ROWS 4

typedef int(*KeyPressCheckCallback)(GLFWwindow*, int);

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

struct Key{
    int keyCode;
    bool keyWasPressed;
    bool keyWasReleased;
    Key(int keyCode){ 
        this->keyCode = keyCode;
        this->keyWasPressed = false;
        this->keyWasReleased = false;
    }
    Key(){
        this->keyCode = 0;
        this->keyWasPressed = false;
        this->keyWasReleased = false;
    }
};

struct Card{
    Suit suit;
    Rank rank;
    glm::vec3 position;
    glm::vec3 scale;
    bool isFlipped;
    AnimationRotate rotateAnimation;
    AnimationChime chimeAnimation;
    float rotateY = 180.0f;
    Card(Suit suit,
         Rank rank,
         glm::vec3 position,
         glm::vec3 scale){
        this->suit = suit;
        this->rank = rank;
        this->isFlipped = false;
        this->position = position;
        this->scale = scale;
    }
    Card(){
        this->suit = SUIT_NONE;
        this->rank = RANK_NONE;
        this->isFlipped = false;
        this->position = glm::vec3(0.0f);
        this->scale = glm::vec3(1.0f);
    }
};
//Input functions
void UpdateInputState(GLFWwindow * window, Key * keys, int numberOfKeys,
                      KeyPressCheckCallback keyPressCheck);
bool GetKeyFromArray(int keyCode, Key * keys, int numberOfKeys, Key * out);
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

float LinearInterpolation(float first, float second, float weight){
    
    float result = weight  * second + (1.0f - weight) * first;
    return result;
}

float CosineInterpolation(float first, float second, float weight){
    float modifiedWeight = (1 - cos(weight * 3.14))/2.0f;
    modifiedWeight = pow(modifiedWeight, 0.2f);
    float result = modifiedWeight  * second + (1.0f - modifiedWeight) * first;
    return result;
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
    
    Card card;
    Object3D obj1;
    Object3D obj2;
    const float scale = 30.0f;
    const float offsetX = 10.0f;
    const float offsetY = 10.0f;
    card.scale = glm::vec3(scale, scale, 1.0f);
    card.position = glm::vec3(scale + offsetX, scale + offsetY, 0.0f);
    obj1.Scale(glm::vec3(scale, scale, 1.0f));
    obj2.Scale(glm::vec3(scale, scale, 1.0f));
    obj1.Translate(glm::vec3(scale + offsetX, scale + offsetY, 0.0f));
    obj2.Translate(glm::vec3(scale + offsetX, scale + offsetY, 0.0f));
    obj1.rotation = 180.0f;
    while(!glfwWindowShouldClose(window)){
        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - time;
        time = currentTime;
        UpdateAnimationState(&card, elapsedTime);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        textures[0].Attach();
        obj1.Draw(&shader, &vao);
        cardBack.Attach();
        obj2.Draw(&shader, &vao);
        UpdateInputState(window, 
                         keyboardKeys,
                         SIZE_OF_ARRAY(keyboardKeys, Key),
                         glfwGetKey);
        UpdateInputState(window,
                         mouseKeys,
                         SIZE_OF_ARRAY(mouseKeys, Key),
                         glfwGetMouseButton);
        Key mouseKey;
        if(GetKeyFromArray(GLFW_MOUSE_BUTTON_LEFT,
                           mouseKeys,
                           SIZE_OF_ARRAY(mouseKeys, Key),
                           &mouseKey)){
            if(mouseKey.keyWasReleased){
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                OpenglCoords coordsOpengl = ScreenToOpenglCoords(x, y);
                std::cout << coordsOpengl.x << "," << coordsOpengl.y << std::endl;
                if(CardWasClicked(&card, coordsOpengl)){
                    if(!card.rotateAnimation.isActive){
                        card.rotateAnimation.isActive = true;
                        obj2.targetRotation = obj2.rotation + 180.0f;
                        obj1.targetRotation = obj1.rotation + 180.0f;
                    }
                }
            }
        }
        if(card.rotateAnimation.isActive){
            float weight = card.rotateAnimation.timeAccumulator / card.rotateAnimation.totalTime;
            obj2.rotation = CosineInterpolation(obj2.targetRotation - 180.0f, obj2.targetRotation, weight);
            obj1.rotation = CosineInterpolation(obj1.targetRotation - 180.0f, obj1.targetRotation, weight);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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

void UpdateInputState(GLFWwindow * window,
                      Key * keys,
                      int numberOfKeys,
                      KeyPressCheckCallback functionToCheckPresses){
    for(int i = 0; i<numberOfKeys; i++){
        Key * currentKey = keys + i;
        int state = functionToCheckPresses(window, currentKey->keyCode);
        if(state == GLFW_PRESS){
            currentKey->keyWasPressed = true;
            currentKey->keyWasReleased = false;
        }
        else if(state == GLFW_RELEASE){
            if(currentKey->keyWasPressed == true){
                currentKey->keyWasReleased = true;
            }
            else{
                currentKey->keyWasReleased = false;
            }
            currentKey->keyWasPressed = false;
        }
    }
}

bool GetKeyFromArray(int keyCode, Key * keys, int numberOfKeys, Key * out){
    for(int i = 0; i<numberOfKeys; i++){
        Key key = keys[i];
        if (key.keyCode == keyCode){
            *out = key;
            return true;
        }
    }
    return false;
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