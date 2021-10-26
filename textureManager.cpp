#include "textureManager.h"

bool TextureManager::LoadTexture(const char * filePath, const char * textureName, int desiredChannels){
    Assert(this->numberOfTextures < MAX_TEXTURES);
    bool textureLoaded = this->textures[this->numberOfTextures].Load(filePath, textureName, desiredChannels);
    if(textureLoaded) this->numberOfTextures++;
    return textureLoaded;
}

bool TextureManager::GetTexture(const char * textureName, Texture * out){
    for(int i = 0; i<this->numberOfTextures; i++){
        if(this->textures[i].textureName.compare(textureName) == 0){
            *out = this->textures[i];
            return true;
        }
    }
    out = NULL;
    return false;
}