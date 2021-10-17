#include <iostream>
#include <windows.h>
#include <cmath>
#include <string>
#include "win32_fileapi.h"
#include "openglIncludes.h"
#include "glmIncludes.h"
#include "shader.h"
#include "vao.h"
#include "texture.h"
#include "model.h"
#include "common.h"
#include "card.h"

#define SIZE_OF_ARRAY(array, type) sizeof(array) / sizeof(type)

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

void UpdateInputState(GLFWwindow * window, Key * keys, int numberOfKeys);
bool GetKeyFromArray(int keyCode, Key * keys, int numberOfKeys, Key * out);

bool LoadTextures(Texture * textures);
//Globals
static float globalAspectRatio = 1.0f;
static bool globalRescale = false;

void FrameBufferSizeCallback(GLFWwindow * window, int width, int height){
    globalAspectRatio = (float)width / height;
    globalRescale = true;
    glViewport(0, 0, width, height);
}


int CALLBACK WinMain(HINSTANCE instance,
					 HINSTANCE prevInstance,
					 LPSTR commandLine,
					 int showCode) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow * window = glfwCreateWindow(800, 800, "CardsGame", NULL, NULL);
    if ( window == NULL ) {
        std::cout << "Failed";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    Shader shader("../shaders/vertexShader.vert", "../shaders/fragmentShader.vert");
    if (!shader.CompileAndLink()){
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
    Texture cardBack("../assets/card-back2.png", 4);
    if (!cardBack.Load()){
        glfwTerminate();
        return -1;
    }
    Card card(DIAMONDS,
              RANK_QUEEN,
              glm::vec3(-100.0f, -100.0f, 0.0f),
              glm::vec3(50.0f, 50.0f * globalAspectRatio, 0.0f));
    card.Initialize();
    shader.Attach(); 
    vao.Attach();
    glm::mat4 projectionMat =  glm::ortho(-400.0f, 400.0f, 400.0f, -400.0f, -500.0f, 500.0f); 
    glm::mat4 camMat = glm::mat4(1.0f);
    shader.SetMat4("uProjectionMat", projectionMat);
    shader.SetMat4("uCamMat", camMat);
    int counter = 90;
    double time = glfwGetTime();
    glfwSwapInterval(1);
    
    Key keys[] = {
        Key(GLFW_KEY_LEFT),
        Key(GLFW_KEY_RIGHT),
        Key(GLFW_KEY_UP),
        Key(GLFW_KEY_DOWN)
    };
    
    while(!glfwWindowShouldClose(window)){
        if(globalRescale){
            card.SetScale(glm::vec3(50.0f, 50.0f * globalAspectRatio, 0.0f));
            globalRescale = false;
        }
        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - time;
        time = currentTime;
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int index = card.cardRank + card.suit * 13;
        textures[index].Attach();
        card.back.Draw(&shader, &vao);
        cardBack.Attach();
        //card.front.Draw(&shader, &vao);
        UpdateInputState(window, keys, SIZE_OF_ARRAY(keys, Key));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void UpdateInputState(GLFWwindow * window, Key * keys, int numberOfKeys){
    for(int i = 0; i<numberOfKeys; i++){
        Key * currentKey = keys + i;
        int state = glfwGetKey(window, currentKey->keyCode);
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
