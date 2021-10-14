#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad.h>
#include <glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture{
    public:
    Texture(const char * filePath, int desiredChannels);
    bool Load();
    void Attach();
    private:
    const char * filePath;
    unsigned int id;
    int height;
    int width;
    int desiredChannels;
};

#endif //TEXTURE_H
