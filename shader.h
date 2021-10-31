#ifndef SHADER_H
#define SHADER_H

#include "debug.h"
#include "win32_fileapi.h"
#include "openglIncludes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#define COMPILE_LINK_SHADER(name) bool name(Shader * shader, const char * vertexShaderPath, const char * fragmentShaderPath, const char * shaderName)
#define ATTACH_SHADER(name) void name(Shader * shader)
#define SET_INT_SHADER(name) void name(Shader * shader, const char * uniformName, int value)
#define SET_FLOAT_SHADER(name) void name(Shader * shader, const char * uniformName, float value)
#define SET_BOOL_SHADER(name) void name(Shader * shader, const char * uniformName, bool value)
#define SET_VEC3_SHADER(name) void name(Shader * shader, const char * uniformName, glm::vec3 vector)
#define SET_VEC4_SHADER(name) void name(Shader * shader, const char * uniformName, glm::vec4 vector)
#define SET_MAT4_SHADER(name) void name(Shader * shader, const char * uniformName, glm::mat4 mat)

struct Shader {
    bool compiledAndLinked = false;
    int vertexShader, fragmentShader, program;
    char infoLog[512];
    std::string name;
};
typedef COMPILE_LINK_SHADER(CompileAndLinkShaderCallback);
typedef ATTACH_SHADER(AttachShaderCallback);
typedef SET_FLOAT_SHADER(SetFloatShaderCallback);
struct ShaderLoadInfo {
    std::string name;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
};

COMPILE_LINK_SHADER(CompileAndLinkShader);
ATTACH_SHADER(AttachShader);
SET_INT_SHADER(SetIntShader);
SET_FLOAT_SHADER(SetFloatShader);
SET_BOOL_SHADER(SetBoolShader);
SET_VEC3_SHADER(SetVec3Shader);
SET_VEC4_SHADER(SetVec4Shader);
SET_MAT4_SHADER(SetMat4Shader);
#endif //SHADER_H
