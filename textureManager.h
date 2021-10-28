#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "texture.h"
#include <vector>
#include <string>
#define MAX_TEXTURES 90
#define TEXTURE_MANAGER_GET_TEXTURE(name) bool name(TextureManager * textureManager, const char * textureName, Texture * out)

struct TextureManager {
    int numberOfTextures = 0;
    Texture textures[MAX_TEXTURES];
};

typedef TEXTURE_MANAGER_GET_TEXTURE(GetTextureTextureManagerCallback);

void LoadTexturesTextureManager(TextureManager * textureManager, std::vector<TextureLoadInfo> textures);
bool LoadTextureTextureManager(TextureManager * textureManager, const char * filePath, const char * textureName, int desiredChannels);
bool GetTextureTextureManager(TextureManager * textureManager, const char * textureName, Texture * out);

#endif