#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include "shader.h"

struct ShaderManager {
    std::vector<Shader> shaders;
    std::vector<Shader> failedShaders;
    void LoadShaders(std::vector<ShaderLoadInfo> shaders);
    void LoadShader(ShaderLoadInfo shader);
    bool GetShader(const char * name, Shader * out);
    bool GetShader(std::string name, Shader * out);
    bool AttachShader(const char * name);
};

#endif