#include "./TransformComponent.h"

#include "../Game.h"

TransformComponent::TransformComponent(Entity* owner, int posX, int posY, int velX, int velY, int w, int h, int s): Component(owner) {
    position = glm::vec2(posX, posY);
    velocity = glm::vec2(velX, velY);
    width = w;
    height = h;
    scale = s;
}

void TransformComponent::Initialize() {

}

void TransformComponent::Update(float deltaTime) {
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

void TransformComponent::Render() {
    
}
