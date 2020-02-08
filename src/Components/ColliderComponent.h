#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"

class ColliderComponent: public Component {
public:
    std::string colliderTag;
    SDL_Rect collider;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    TransformComponent* transform;

    ColliderComponent(Entity* owner, std::string colliderTag, int x, int y, int width, int height);

    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;
};

#endif