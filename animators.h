
#ifndef ANIMATORS_H
#define ANIMATORS_H

#include "misc.h"
namespace Anim{
    struct AnimationRotate {
        float timeAccumulator = 0.0f;
        bool isActive = false;
        float totalTime = .5f;
        float startingValue = 0.0f;
        float endingValue = 0.0f;
        float currentValue = 0.0f;
        void Update(float deltaTime);
    };

    void UpdateRotateAnimationState(AnimationRotate * animation, float deltaTime);
    void UpdateRotateAnimation(AnimationRotate * animation);
    void ActivateRotateAnimation(AnimationRotate * animation);
};
#endif //ANIMATORS_H

