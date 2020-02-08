#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H

#include "../../lib/glm/glm.hpp"
#include "../EntityManager.h"
#include "./TransformComponent.h"

class ProjectileEmitterComponent: public Component {
private:
    TransformComponent* transform;
    glm::vec2 origin;
    int speed;
    int range;
    float angle; //radian
    bool loop;

public:
    ProjectileEmitterComponent(int speed, int angle, int range, bool loop);

    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;
};

#endif