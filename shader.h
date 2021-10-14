#ifndef SHADER_H
#define SHADER_H

#include "win32_fileapi.h"
#include <glad.h>
#include <glfw3.h>

class Shader {
    public:
    Shader(const char * vertexShaderPath,
           const char * fragmentShaderPath);
    bool CompileAndLink();
    void Attach();
    private:
    const char * vertexShaderPath;
    const char * fragmentShaderPath;
    win32_file vertexShaderSource;
    win32_file fragmentShaderSource;
    int vertexShader, fragmentShader, program;
    char infoLog[512];
    
};

#endif //SHADER_H
