#include "texture.h"

Texture::Texture(const char * filePath, const char * textureName, int desiredChannels){
    this->filePath = filePath;
    this->desiredChannels = desiredChannels;
    this->textureName = std::string(textureName);
    this->loadable = true;
    this->loaded = false;
}

Texture::Texture(){
    this->loadable = false;
    this->loaded = false;
}

bool Texture::Load(const char * filePath, const char * textureName, int desiredChannels){
    this->filePath = filePath;
    this->desiredChannels = desiredChannels;
    this->textureName = std::string(textureName);
    this->loadable = true;
    return this->Load();
}

bool Texture::Load(){
    Assert(this->loadable);
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char *data = stbi_load(this->filePath,
                                    &this->width,
                                    &this->height, 
                                    &this->desiredChannels, 0);
    int channels = this->desiredChannels == 4 ? GL_RGBA : GL_RGB;
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, channels, this->width, this->height, 0, channels, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        this->loaded = true;
        return true;
    }
    return false;
}

void Texture::Attach(){
    Assert(this->loaded);
    glBindTexture(GL_TEXTURE_2D, this->id);
}