#include "misc.h"

float LinearInterpolation(float first, float second, float weight){
    
    float result = weight  * second + (1.0f - weight) * first;
    return result;
}

float CosineInterpolation(float first, float second, float weight){
    float modifiedWeight = (1 - cos(weight * 3.14))/2.0f;
    modifiedWeight = pow(modifiedWeight, 0.2f);
    float result = modifiedWeight  * second + (1.0f - modifiedWeight) * first;
    return result;
}

