#include <iostream>
#include <windows.h>
#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "win32_fileapi.h"
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
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f
    };
    VertexArrayObject vao(vertices, 30);
    Texture tex("../assets/smiley.png", 4);
    if (!tex.Load()){
        glfwTerminate();
        return -1;
    }
    Object3D obj("SomeObject", &vao);
    obj.Scale(glm::vec3(200.0f, 200.0f, 1.0f));
    obj.Translate(glm::vec3(100.0f, 0.0f, 0.0f));
    shader.Attach();
    vao.Attach();
    tex.Attach();
    glm::mat4 projectionMat =  glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -500.0f, 500.0f); 
    glm::mat4 camMat = glm::mat4(1.0f);
    glm::mat4 modelMat = obj.GetModelMat();
    shader.SetMat4("uProjectionMat", projectionMat);
    int counter = 0;
    while(!glfwWindowShouldClose(window)){
        modelMat = obj.GetModelMat();
        obj.Rotate(counter * 0.001f);
        shader.SetMat4("uCamMat", camMat);
        shader.SetMat4("uModelMat", modelMat);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
        counter++;
    }
    glfwTerminate();
    return 0;
}