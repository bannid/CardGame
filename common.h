#ifndef COMMON_H
#define COMMON_H

#define CNT_ARR(array) sizeof(array) /sizeof(array[0])

struct GlobalInfo {
    float windowHeight;
    float windowWidth;
    float openglWidth;
    float openglHeight;
    float aspectRatio;
    bool rescale;
};

#endif 