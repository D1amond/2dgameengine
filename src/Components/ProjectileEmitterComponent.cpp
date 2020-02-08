#include "./ProjectileEmitterComponent.h"

ProjectileEmitterComponent::ProjectileEmitterComponent(int speed, int angle, int range, bool loop) {
    this->speed = speed;
    this->range = range;
    this->loop = loop;
    this->angle = glm::radians(static_cast<float>(angle));
}

void ProjectileEmitterComponent::Initialize() {
    transform = owner->GetComponent<TransformComponent>();
    origin = glm::vec2(transform->position.x, transform->position.y);
    transform->velocity = glm::vec2(glm::cos(angle) * speed, glm::sin(angle) * speed);
}

void ProjectileEmitterComponent::Update(float deltaTime) {
    if (glm::distance(transform->position, origin) > range) {
        if (loop) {
            transform->position.x = origin.x;
            transform->position.y = origin.y;
        } else {
            owner->Destroy();
        }
    }
}

void ProjectileEmitterComponent::Render() {

}
