#ifndef SHADER_H
#define SHADER_H

#include "win32_fileapi.h"
#include "openglIncludes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
    public:
    Shader(const char * vertexShaderPath,
           const char * fragmentShaderPath);
    bool CompileAndLink();
    void Attach();
    void SetInt(const char * name, int value)const;
    void SetFloat(const char * name, float value)const;;
    void SetBool(const char * name, bool value)const;
    void SetVec3(const char * name, glm::vec3 vector)const;
    void SetVec4(const char * name, glm::vec4 vector)const;
    void SetMat4(const char * name, glm::mat4 mat)const;
    private:
    const char * vertexShaderPath;
    const char * fragmentShaderPath;
    win32_file vertexShaderSource;
    win32_file fragmentShaderSource;
    int vertexShader, fragmentShader, program;
    char infoLog[512];
    
};

#endif //SHADER_H
