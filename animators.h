
#ifndef ANIMATORS_H
#define ANIMATORS_H

struct AnimationRotate {
    float timeAccumulator = 0.0f;
    bool isActive = false;
    float totalTime = .5f;
    float startingValue = 0.0f;
    float endingValue = 0.0f;
    
};

#endif //ANIMATORS_H

