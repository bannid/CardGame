#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include "shader.h"

#define GET_SHADER_SHADER_MANAGER(name) bool name(ShaderManager * sm, const char * shaderName, Shader * out)
#define LOAD_SHADER_SHADER_MANAGER(name) void name(ShaderManager * sm, ShaderLoadInfo shaderInfo)
#define LOAD_SHADERS_SHADER_MANAGER(name) void name(ShaderManager * sm, std::vector<ShaderLoadInfo> shaders)
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
typedef LOAD_SHADER_SHADER_MANAGER(LoadShaderShaderManagerCallback);
typedef LOAD_SHADERS_SHADER_MANAGER(LoadShadersShaderManagerCallback);
GET_SHADER_SHADER_MANAGER(GetShaderShaderManager);
LOAD_SHADER_SHADER_MANAGER(LoadShaderShaderManager);
LOAD_SHADERS_SHADER_MANAGER(LoadShadersShaderManager);

#endif