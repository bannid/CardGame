#include "shaderManager.h"

void ShaderManager::LoadShaders(std::vector<ShaderLoadInfo> shaders){
    for(auto i = shaders.begin(); i != shaders.end(); i++){
        Shader shader;
        if(CompileAndLinkShader(&shader, i->vertexShaderPath.c_str(), i->fragmentShaderPath.c_str(), i->name.c_str())) {
            this->shaders.push_back(shader);
        }
        else { 
            this->failedShaders.push_back(shader);
        }
    }
}

void ShaderManager::LoadShader(ShaderLoadInfo shader){
    Shader shaderTemp;
    if(CompileAndLinkShader(&shaderTemp, shader.vertexShaderPath.c_str(), shader.fragmentShaderPath.c_str(), shader.name.c_str())){
        this->shaders.push_back(shaderTemp);
    }
    else{
        this->failedShaders.push_back(shaderTemp);
    }
}

bool ShaderManager::GetShader(const char * shaderName, Shader * out){
    return this->GetShader(std::string(shaderName), out);
}

bool ShaderManager::GetShader(std::string shaderName, Shader * out){
    for(auto i = shaders.begin(); i != shaders.end(); i++){
        if(i->name.compare(shaderName) == 0){
            *out = *i;
            return true;
        }
    }
    return false;
}

bool ShaderManager::AttachShaderShaderManager(const char * name){
    Shader shader;
    if(this->GetShader(name, &shader)){
        AttachShader(&shader);
        return true;
    }
    return false;
}

bool GetShaderShaderManager(ShaderManager * sm, const char * shaderName, Shader * out){
    for(auto i = sm->shaders.begin(); i != sm->shaders.end(); i++){
        if(i->name.compare(shaderName) == 0){
            *out = *i;
            return true;
        }
    }
    return false;
}

void LoadShaderShaderManager(ShaderManager * sm, ShaderLoadInfo shaderInfo){
    sm->LoadShader(shaderInfo);
}

void LoadShadersShaderManager(ShaderManager * sm, std::vector<ShaderLoadInfo> shaders){
    for(auto i = shaders.begin(); i != shaders.end(); i++){
        Shader shader;
        if(CompileAndLinkShader(&shader, i->vertexShaderPath.c_str(), i->fragmentShaderPath.c_str(), i->name.c_str())) {
            sm->shaders.push_back(shader);
        }
        else { 
            sm->failedShaders.push_back(shader);
        }
    }
}
