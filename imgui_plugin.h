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
#elif !defined(DEBUG)
    #define IMGUI_INIT(windowPtr) 
    #define IMGUI_NEW_FRAME() 
    #define IMGUI_RENDER() 
    #define IMGUI_EXIT() 
    #define IMGUI_FUNCTION(name) 
#endif

void ImguiInit(GLFWwindow * window);
void ImguiNewFrame();
void ImguiRender();
void ImguiExit();

#endif