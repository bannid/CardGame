
#ifndef ANIMATORS_H
#define ANIMATORS_H

struct AnimationRotate {
    float timeAccumulator = 0.0f;
    bool isActive = false;
    float totalTime = .5f;
    
    
};

struct AnimationChime {
    float timeAccumulator = 0.0f;
    bool isActive = false;
    float totalTime = 2.0f;
};

#endif //ANIMATORS_H

