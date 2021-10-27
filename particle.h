#ifndef PARTICLE_H
#define PARTICLE_H

#include "glmIncludes.h"

namespace Effects{
    struct Particle {
        glm::vec3 pos;
        glm::vec3 velocity;
        float timeToLive = 0;
        float elapsedTime = 0;
        bool isAlive = false;
    };
    void ActivateParticle(Particle * particle, glm::vec3 pos, glm::vec3 velocity, float timeToLive);
    void UpdateParticle(Particle * particle, float deltaTime);
};
#endif