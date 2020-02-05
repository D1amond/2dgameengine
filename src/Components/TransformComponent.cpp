#include "./TransformComponent.h"

#include "../Game.h"

TransformComponent::TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s) {
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
    SDL_Rect transformRectangle = {
        (int) position.x,
        (int) position.y,
        width,
        height
    };

    SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(Game::renderer, &transformRectangle);
}
