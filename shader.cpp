#include "shader.h"

Shader::Shader(const char * vertexShaderPath, const char * fragmentShaderPath){
    this->fragmentShaderPath = fragmentShaderPath;
    this->vertexShaderPath = vertexShaderPath;
}

bool Shader::CompileAndLink(){
    if (!read_entire_file(this->vertexShaderPath, &this->vertexShaderSource)){
        return false;
    }
    
    if (!read_entire_file(this->fragmentShaderPath, &this->fragmentShaderSource)){
        close_file(&this->vertexShaderSource);
        return false;
    }
    int success;
    this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(this->vertexShader, 1,(const char**) &this->vertexShaderSource, NULL);
    glCompileShader(this->vertexShader);
    glGetShaderiv(this->vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(this->vertexShader, 512, NULL, this->infoLog);
        close_file(&this->vertexShaderSource);
        close_file(&this->fragmentShaderSource);
        return false;
    }
    this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(this->fragmentShader, 1, (const char**) &this->fragmentShaderSource, NULL);
    glCompileShader(this->fragmentShader);
    glGetShaderiv(this->fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(this->fragmentShader, 512, NULL, this->infoLog);
        close_file(&this->vertexShaderSource);
        close_file(&this->fragmentShaderSource);
        return false;
    }
    //Close the files
    close_file(&this->vertexShaderSource);
    close_file(&this->fragmentShaderSource);
    
    this->program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    //Delete the shaders
    glDeleteShader(this->vertexShader);
    glDeleteShader(this->fragmentShader);
    if (!success){
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        return false;
    }
    return true;
}

void Shader::Attach() {
    glUseProgram(this->program);
}