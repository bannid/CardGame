#include "particle.h"

namespace Effects {
    void ActivateParticle(Particle * particle,
        glm::vec3 pos, glm::vec3 velocity,
        float timeToLive){
        particle->velocity = velocity;
        particle->pos = pos;
        particle->timeToLive = timeToLive;
        particle->elapsedTime = 0.0f;
        particle->isAlive = true;
    }
    
    void UpdateParticle(Particle * particle, float deltaTime){
        particle->pos += particle->velocity * deltaTime;
        particle->elapsedTime += deltaTime;
        if(particle->elapsedTime >= particle->timeToLive){
            particle->isAlive = false;
        }
    }
};
