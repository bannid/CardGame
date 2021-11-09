#ifndef TEXTURE_H
#define TEXTURE_H
#include "openglIncludes.h"
#include "stb_image.h"
#include "debug.h"
#include <string>

#define TEXTURE_ATTACH_FUNCTION(name) void name(Texture * texture)
#define TEXTURE_LOAD_FUNCTION(name) bool name(Texture * texture, const char * filePath, const char * textureName, int desiredChannels, bool flip)

struct Texture{
    public:
    std::string     textureName;
    const char*     filePath;
    unsigned int    id;
    int             height;
    int             width;
    int             desiredChannels;
    bool            loaded;
};
typedef TEXTURE_ATTACH_FUNCTION(AttachTextureCallback);
typedef TEXTURE_LOAD_FUNCTION(LoadTextureCallback);
struct TextureAPI {
    AttachTextureCallback * attachCallback;
    LoadTextureCallback * loadCallback;
};
struct TextureLoadInfo {
    std::string textureName;
    std::string name;
    int desiredChannels;
};

TEXTURE_LOAD_FUNCTION(LoadTexture);
TEXTURE_ATTACH_FUNCTION(AttachTexture);
#endif //TEXTURE_H
