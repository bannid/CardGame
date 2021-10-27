#ifndef IMGUI_PLUGIN_H
#define IMGUI_PLUGIN_H

#include "openglIncludes.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#ifdef DEBUG
    #define IMGUI_INIT(windowPtr) ImguiInit(windowPtr)
    #define IMGUI_NEW_FRAME() ImguiNewFrame()
    #define IMGUI_RENDER() ImguiRender()
    #define IMGUI_EXIT() ImguiExit()
    #define IMGUI_FUNCTION(name) name
#endif
#ifndef DEBUG
    #define IMGUI_INIT(windowPtr) 
    #define IMGUI_NEW_FRAME() 
    #define IMGUI_RENDER() 
    #define IMGUI_EXIT() 
    #define IMGUI_FUNCTION(name) 
#endif

void ImguiInit(GLFWwindow * window){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}
void ImguiNewFrame(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void ImguiRender(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void ImguiExit(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

#endif