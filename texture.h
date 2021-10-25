#ifndef TEXTURE_H
#define TEXTURE_H
#include "openglIncludes.h"
#include "stb_image.h"
#include "debug.h"
#include <string>

class Texture{
    public:
    Texture(const char * filePath, const char * textureName, int desiredChannels);
    Texture();
    bool Load();
    bool Load(const char * filePath, const char * textureName, int desiredChannels);
    void Attach();
    private:
    std::string textureName;
    const char * filePath;
    unsigned int id;
    int height;
    int width;
    int desiredChannels;
    bool loadable;
    bool loaded;
};

#endif //TEXTURE_H
