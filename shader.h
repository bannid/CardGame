#ifndef SHADER_H
#define SHADER_H

#include "debug.h"
#include "win32_fileapi.h"
#include "openglIncludes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

struct Shader {
    Shader(const char * vertexShaderPath,
           const char * fragmentShaderPath,
           const char * name);
    Shader();
    bool CompileAndLink();
    void Attach();
    void SetInt(const char * name, int value);
    void SetFloat(const char * name, float value);
    void SetBool(const char * name, bool value);
    void SetVec3(const char * name, glm::vec3 vector);
    void SetVec4(const char * name, glm::vec4 vector);
    void SetMat4(const char * name, glm::mat4 mat);
    bool compiledAndLinked;
    const char * vertexShaderPath;
    const char * fragmentShaderPath;
    win32_file vertexShaderSource;
    win32_file fragmentShaderSource;
    int vertexShader, fragmentShader, program;
    char infoLog[512];
    std::string name;
};

struct ShaderLoadInfo {
    std::string name;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
};

#endif //SHADER_H
