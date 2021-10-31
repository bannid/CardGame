#include "shader.h"

void AttachShader(Shader * shader) {
    Assert(shader->compiledAndLinked);
    glUseProgram(shader->program);
}

void SetIntShader(Shader * shader, const char * name, int value){
    AttachShader(shader);
    glUniform1i(glGetUniformLocation(shader->program, name), value);
}

void SetBoolShader(Shader * shader, const char * name, bool value){
    AttachShader(shader);
    glUniform1i(glGetUniformLocation(shader->program, name), value);
}

void SetFloatShader(Shader * shader, const char * name, float value){
    AttachShader(shader);
    glUniform1f(glGetUniformLocation(shader->program, name), value);
}

void SetVec3Shader(Shader * shader, const char * name, glm::vec3 vector){
    AttachShader(shader);
    glUniform3fv(glGetUniformLocation(shader->program, name),
                 1,
                 glm::value_ptr(vector));
}

void SetVec4Shader(Shader * shader, const char * name, glm::vec4 vector){
    AttachShader(shader);
    glUniform4fv(glGetUniformLocation(shader->program, name),
                 1,
                 glm::value_ptr(vector));
}

void SetMat4Shader(Shader * shader, const char * name, glm::mat4 mat){
    AttachShader(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader->program, name), 1, GL_FALSE, glm::value_ptr(mat));
}

bool CompileAndLinkShader(Shader * shader, const char * vertexShaderPath, const char * fragmentShaderPath, const char * shaderName){
    shader->name = std::string(shaderName);
    win32_file vertexShaderSource;
    win32_file fragmentShaderSouce;
    if (!read_entire_file(vertexShaderPath, &vertexShaderSource)){
        return false;
    }
    
    if (!read_entire_file(fragmentShaderPath, &fragmentShaderSouce)){
        close_file(&vertexShaderSource);
        return false;
    }
    int success;
    //Vertex shader
    shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader->vertexShader, 1,(const char**) &vertexShaderSource, NULL);
    glCompileShader(shader->vertexShader);
    glGetShaderiv(shader->vertexShader, GL_COMPILE_STATUS, &success);
    //Check if the vertex shader compiled
    if (!success){
        glGetShaderInfoLog(shader->vertexShader, 512, NULL, shader->infoLog);
        close_file(&vertexShaderSource);
        close_file(&fragmentShaderSouce);
        return false;
    }
    //Fragment shader
    shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->fragmentShader, 1, (const char**) &fragmentShaderSouce, NULL);
    glCompileShader(shader->fragmentShader);
    glGetShaderiv(shader->fragmentShader, GL_COMPILE_STATUS, &success);
    //Check if the fragment shader compiled successfully
    if (!success){
        glGetShaderInfoLog(shader->fragmentShader, 512, NULL, shader->infoLog);
        close_file(&vertexShaderSource);
        close_file(&fragmentShaderSouce);
        return false;
    }
    //Close the files
    close_file(&vertexShaderSource);
    close_file(&fragmentShaderSouce);
    
    shader->program = glCreateProgram();
    glAttachShader(shader->program, shader->vertexShader);
    glAttachShader(shader->program, shader->fragmentShader);
    glLinkProgram(shader->program);
    glGetProgramiv(shader->program, GL_LINK_STATUS, &success);
    //Delete the shaders
    glDeleteShader(shader->vertexShader);
    glDeleteShader(shader->fragmentShader);
    if (!success){
        glGetProgramInfoLog(shader->program, 512, NULL, shader->infoLog);
        return false;
    }
    shader->compiledAndLinked = true;
    return true;
}