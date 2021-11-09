#include "textureManager.h"

void LoadTexturesTextureManager(TextureManager * textureManager, std::vector<TextureLoadInfo> textures){
    for(auto i = textures.begin(); i != textures.end(); i++){
        
    }
}

bool LoadTextureTextureManager(TextureManager * textureManager, const char * filePath, const char * textureName, int desiredChannels){
    Assert(textureManager->numberOfTextures < MAX_TEXTURES);
    bool textureLoaded = LoadTexture(textureManager->textures + textureManager->numberOfTextures, filePath, textureName, desiredChannels, true);
    if(textureLoaded) textureManager->numberOfTextures++;
    return textureLoaded;
}

bool GetTextureTextureManager(TextureManager * textureManager, const char * textureName, Texture * out){
    for(int i = 0; i<textureManager->numberOfTextures; i++){
        if(textureManager->textures[i].textureName.compare(textureName) == 0){
            *out = textureManager->textures[i];
            return true;
        }
    }
    out = NULL;
    return false;
}