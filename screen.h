#ifndef SCREEN_H
#define SCREEN_H

#include "common.h"

struct OpenglCoords { float x; float y; };
OpenglCoords ScreenToOpenglCoords(float x, float y, GlobalInfo * globalInfo); 
bool RectangleWasClicked(float leftX, 
                         float leftY, 
                         float rightX, 
                         float rightY, 
                         OpenglCoords coords);
#endif