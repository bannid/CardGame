#ifndef TEXT_H
#define TEXT_H

#include <vector>
#include <sstream>
#include "texture.h"
#include "model.h"
#include "vao.h"
#include "typedefines.h"
#include "win32_fileapi.h"

#define RENDER_TEXT_FUNC(name) void name(std::string text, CharacterSet * characterSet, int size, TextAlign alignment, glm::vec2 pos)

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
    Texture * fontAtlasTexture = NULL;
    Quad * quad = NULL;
    Shader * shader = NULL;
    VertexArrayObject * vao = NULL;
};

enum TextAlign{
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
};
typedef RENDER_TEXT_FUNC(RenderTextCallback);
bool LoadFonts(const char * filePath, CharacterSet * characterSet);
RENDER_TEXT_FUNC(RenderText);
#endif