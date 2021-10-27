#include "shaderManager.h"

void ShaderManager::LoadShaders(std::vector<ShaderLoadInfo> shaders){
    for(auto i = shaders.begin(); i != shaders.end(); i++){
        Shader shader(i->vertexShaderPath.c_str(), i->fragmentShaderPath.c_str(), i->name.c_str());
        if(shader.CompileAndLink()) {
            this->shaders.push_back(shader);
        }
        else { 
            this->failedShaders.push_back(shader);
        }
    }
}

void ShaderManager::LoadShader(ShaderLoadInfo shader){
    Shader shaderTemp(shader.vertexShaderPath.c_str(), shader.fragmentShaderPath.c_str(), shader.name.c_str());
    if(shaderTemp.CompileAndLink()){
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

bool ShaderManager::AttachShader(const char * name){
    Shader shader;
    if(this->GetShader(name, &shader)){
        shader.Attach();
        return true;
    }
    return false;
}