#include "text.h"
bool StringContainsString(std::string string, std::string match){
    if(string.length() < match.length()){
        return false;
    }
    for(int i = 0; i < match.length(); i++){
        if(match[i] != string[i]){
            return false;
        }
    }
    return true;
}

int GetValuesOutOfTokens(std::string string){
    std::vector<std::string> tokens;
    std::istringstream stream(string);
    std::string temp;
    while(std::getline(stream, temp,'=')){
        tokens.push_back(temp);
    }
    Assert(tokens.size() == 2);
    return std::stoi(tokens[1]);
}

bool LoadFonts(const char * filePath, CharacterSet * characterSet){
    win32_file fontFile;
    if(read_entire_file(filePath, &fontFile)){
        std::istringstream stream(fontFile.Data);
        std::istringstream tokenStream;
        std::istringstream valueStream;
        std::string line;
        while(std::getline(stream, line)){
            std::vector<std::string> tokens;
            std::string temp;
            tokenStream = std::istringstream(line.c_str());
            while(std::getline(tokenStream, temp, ' ')){
                if(!temp.empty())tokens.push_back(temp);
            }
            if(tokens[0].compare("char") == 0){
                int id = GetValuesOutOfTokens(tokens[1]);
                int x = GetValuesOutOfTokens(tokens[2]);
                int y = GetValuesOutOfTokens(tokens[3]);
                int width = GetValuesOutOfTokens(tokens[4]);
                int height = GetValuesOutOfTokens(tokens[5]);
                int xOffset = GetValuesOutOfTokens(tokens[6]);
                int yOffset = GetValuesOutOfTokens(tokens[7]);
                int xAdvance = GetValuesOutOfTokens(tokens[8]);
                characterSet->characters[id].id = id;
                characterSet->characters[id].x = (float) x;// todo: get this padding from the file
                characterSet->characters[id].y = (float) y;
                characterSet->characters[id].width = (float) width;
                characterSet->characters[id].height = (float) height;
                characterSet->characters[id].xAdvance = (float) xAdvance;
                characterSet->characters[id].yOffset = (float) yOffset;
                characterSet->characters[id].xOffset= (float) xOffset;
            }
            else if(tokens[0].compare("common") == 0){
                int lineHeight = GetValuesOutOfTokens(tokens[1]);
                int fontAtlasHeight = GetValuesOutOfTokens(tokens[4]);
                int fontAtlasWidth = GetValuesOutOfTokens(tokens[3]);
                characterSet->fontAtlasHeight = fontAtlasHeight;
                characterSet->fontAtlasWidth = fontAtlasWidth;
                characterSet->lineHeight = (float) lineHeight;
            }
        }
        close_file(&fontFile);
    }
    return false;
}