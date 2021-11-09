#include "texture.h"

void AttachTexture(Texture * texture){
    Assert(texture->loaded);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

bool LoadTexture(Texture * texture, const char * filePath, const char * textureName, int desiredChannels, bool flip = true){
    stbi_set_flip_vertically_on_load(flip);
    texture->textureName = textureName;
    texture->desiredChannels = desiredChannels;
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char *data = stbi_load(filePath,
                                    &texture->width,
                                    &texture->height, 
                                    &texture->desiredChannels, 0);
    int channels = texture->desiredChannels == 4 ? GL_RGBA : GL_RGB;
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, channels, texture->width, texture->height, 0, channels, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        texture->loaded = true;
        return true;
    }
    return false;
}