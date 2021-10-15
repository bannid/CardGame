#ifndef TEXTURE_H
#define TEXTURE_H
#include "openglIncludes.h"
#include "stb_image.h"

class Texture{
    public:
    Texture(const char * filePath, int desiredChannels);
    Texture();
    bool Load();
    bool Load(const char * filePath, int desiredChannels);
    void Attach();
    private:
    const char * filePath;
    unsigned int id;
    int height;
    int width;
    int desiredChannels;
};

#endif //TEXTURE_H
