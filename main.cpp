#include <iostream>
#include <windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "win32_fileapi.h"
#include "openglIncludes.h"
#include "shader.h"
#include "vao.h"
#include "texture.h"
#include "model.h"

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
    GLFWwindow * window = glfwCreateWindow(800, 600, "CardsGame", NULL, NULL);
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
    Texture tex2("../assets/card-clubs-1.png", 1);
    if (!tex.Load()){
        glfwTerminate();
        return -1;
    }
    if (!tex2.Load()){
        glfwTerminate();
        return -1;
    }
    Object3D obj("SomeObject", &vao, &shader);
    Object3D obj2("SomeOtherObject", &vao, &shader);
    obj.Scale(glm::vec3(50.0f, 50.0f, 1.0f));
    obj2.Scale(glm::vec3(50.0f, 50.0f, 1.0f));
    obj.Translate(glm::vec3(50.0f, 50.0f, 0.0f));
    obj2.Translate(glm::vec3(50.0f, 50.0f, 0.0f));
    obj2.Rotate(180.0f);
    shader.Attach(); 
    vao.Attach();
    glm::mat4 projectionMat =  glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -500.0f, 500.0f); 
    glm::mat4 camMat = glm::mat4(1.0f);
    glm::mat4 modelMat = obj.GetModelMat();
    shader.SetMat4("uProjectionMat", projectionMat);
    shader.SetMat4("uCamMat", camMat);
    int counter = 90;
    double time = glfwGetTime();
    //glfwSwapInterval(1);
    while(!glfwWindowShouldClose(window)){
        float currentTime = glfwGetTime();
        float elapsedTime = currentTime - time;
        time = currentTime;
        modelMat = obj.GetModelMat();
        obj.Rotate(counter * elapsedTime);
        obj2.Rotate(counter * elapsedTime);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 
        tex.Attach();
        obj.Draw();
        tex2.Attach();
        obj2.Draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}