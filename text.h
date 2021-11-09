#ifndef TEXT_H
#define TEXT_H

#include <vector>
#include <sstream>
#include "texture.h"
#include "typedefines.h"
#include "win32_fileapi.h"

struct Character {
    uint32 id;
    float width;
    float height;
    float x;
    float y;
    float xAdvance;
    float yOffset;
    float xOffset;
};
struct CharacterSet{
    const char * name;
    Character characters[128];
    std::string fileName;
    int fontAtlasHeight = 0;
    int fontAtlasWidth = 0;
    float lineHeight = 0.0f;
    Texture * fontAtlasTexture;
};
bool LoadFonts(const char * filePath, CharacterSet * characterSet);

#endif