#include "animators.h"
namespace Anim{
    void UpdateRotateAnimationState(AnimationRotate * animation, float deltaTime){
        if(animation->isActive){
            if(animation->timeAccumulator >= animation->totalTime){
                animation->isActive = false;
                animation->timeAccumulator = 0.0f;
            }
            else{
                animation->timeAccumulator += deltaTime;
            }
            animation->timeAccumulator = animation->timeAccumulator > animation->totalTime ? animation->totalTime : animation->timeAccumulator;
        }
    }

    void UpdateRotateAnimation(AnimationRotate * animation){
        if(animation->isActive){
            float weight = animation->timeAccumulator / animation->totalTime;
            animation->currentValue = CosineInterpolation(animation->startingValue , animation->endingValue, weight);
        }
    }

    void ActivateRotateAnimation(AnimationRotate * animation){
        if(animation->isActive) return;
        animation->isActive = true;
        animation->startingValue = animation->currentValue;
        animation->endingValue = animation->currentValue + 180.0f;
    }
};