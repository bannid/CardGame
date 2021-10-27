#include "screen.h"

OpenglCoords ScreenToOpenglCoords(float x, float y, GlobalInfo * globalInfo){
    OpenglCoords local;
    local.x = (x / globalInfo->windowWidth) * globalInfo->openglWidth;
    local.y = (y / globalInfo->windowHeight) * globalInfo->openglHeight;
    return local;
} 

bool RectangleWasClicked(float leftX, 
                         float leftY, 
                         float rightX, 
                         float rightY, 
                         OpenglCoords coords){
    bool isInOnXAxis = coords.x >= leftX && coords.x <= rightX;
    bool isInOnYAxis = coords.y >= leftY && coords.y <= rightY;
    return isInOnXAxis && isInOnYAxis;
}