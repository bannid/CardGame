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

void FrameBufferSizeCallback(GLFWwindow * window, int width, int height){
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
    std::string stringOne("card-");
    std::string stringTwo[4] = {
        std::string("diamonds-"),
        std::string("clubs-"),
        std::string("spades-"),
        std::string("hearts-")
    };
    Texture textures[52];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            std::string temp = std::to_string(j + 1);
            std::string filePath = "../assets/pfc/pc/" + 
                stringOne +
                stringTwo[i] +
                temp + ".png";
            int index = j + i * 13;
            if(!textures[index].Load(filePath.c_str(), 4)){
                glfwTerminate();
                return -1;
            }
            
        }
    }
    
    float vertices[] = {
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //top left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //bottom left
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //bottom right
        
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //top left
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //bottom right
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f //top right
    };
    VertexArrayObject vao(vertices, sizeof(vertices)/sizeof(float));
    Texture tex("../assets/card-back2.png", 4);
    if (!tex.Load()){
        glfwTerminate();
        return -1;
    }
    Card card;
    card.Initialize();
    shader.Attach(); 
    vao.Attach();
    glm::mat4 projectionMat =  glm::ortho(-400.0f, 400.0f, 400.0f, -400.0f, -500.0f, 500.0f); 
    glm::mat4 camMat = glm::mat4(1.0f);
    shader.SetMat4("uProjectionMat", projectionMat);
    shader.SetMat4("uCamMat", camMat);
    int counter = 90;
    double time = glfwGetTime();
    //glfwSwapInterval(1);
    while(!glfwWindowShouldClose(window)){
        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - time;
        time = currentTime;
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 
        textures[51].Attach();
        card.Draw(&shader, &vao);
        tex.Attach();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}