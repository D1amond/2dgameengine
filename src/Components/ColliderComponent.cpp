#include "./ColliderComponent.h"

ColliderComponent::ColliderComponent(Entity* owner, std::string colliderTag, int x, int y, int width, int height): Component(owner) {
    this->colliderTag = colliderTag;
    this->collider = {x, y, width, height};
}

void ColliderComponent::Initialize() {
    transform = owner->GetComponent<TransformComponent>();
    sourceRectangle = {0, 0, transform->width, transform->height};
    destinationRectangle = {collider.x, collider.y, collider.w, collider.h};
}

void ColliderComponent::Update(float deltaTime) {
    collider.x = static_cast<int>(transform->position.x);
    collider.y = static_cast<int>(transform->position.y);
    collider.w = transform->width * transform->scale;
    collider.h = transform->height * transform->scale;

    destinationRectangle.x = collider.x - owner->level->camera.x;
    destinationRectangle.y = collider.y - owner->level->camera.y;
}

void ColliderComponent::Render() {
    
}