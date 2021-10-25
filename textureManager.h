#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "texture.h"
#include <string>
#define MAX_TEXTURES 90

struct TextureManager {
    int numberOfTextures = 0;
    Texture textures[MAX_TEXTURES];
    bool LoadTexture(const char * filePath, const char * textureName, int desiredChannels);
    bool GetTexture(const char * textureName, Texture * out);
};

#endif