#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include "shader.h"

#define GET_SHADER_SHADER_MANAGER(name) bool name(ShaderManager * sm, const char * shaderName, Shader * out)

struct ShaderManager {
    std::vector<Shader> shaders;
    std::vector<Shader> failedShaders;
    void LoadShaders(std::vector<ShaderLoadInfo> shaders);
    void LoadShader(ShaderLoadInfo shader);
    bool GetShader(const char * name, Shader * out);
    bool GetShader(std::string name, Shader * out);
    bool AttachShaderShaderManager(const char * name);
};
typedef GET_SHADER_SHADER_MANAGER(GetShaderShaderManagerCallback);
GET_SHADER_SHADER_MANAGER(GetShaderShaderManager);

#endif