#include <iostream>
#include <windows.h>
#include <glad.h>
#include <glfw3.h>
#include "win32_fileapi.h"
#include "shader.h"
#include "vao.h"
#include "texture.h"

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
    GLFWwindow * window = glfwCreateWindow(600, 600, "CardsGame", NULL, NULL);
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
    Shader shader("../shaders/vertexShader.vert", "../shaders/fragmentShader.vert");
    if (!shader.CompileAndLink()){
        glfwTerminate();
        return -1;
    }
    
    float vertices[] = {
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    };
    VertexArrayObject vao(vertices, 15);
    Texture tex("../assets/smiley.png", 4);
    if (!tex.Load()){
        glfwTerminate();
        return -1;
    }
    shader.Attach();
    vao.Attach();
    tex.Attach();
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}