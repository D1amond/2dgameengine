#include "./SpriteComponent.h"

#include <iostream>

SpriteComponent::SpriteComponent(Entity* owner, std::string assetTextureId): Component(owner) {
    isAnimated = false;
    isFixed = false;
    SetTexture(assetTextureId);
}

SpriteComponent::SpriteComponent(Entity* owner, std::string assetTextureId, bool isFixed): Component(owner) {
    this->isAnimated = false;
    this->isFixed = isFixed;
    SetTexture(assetTextureId);
}

SpriteComponent::SpriteComponent(Entity* owner, std::string assetTextureId, int numFrames, int animationSpeed, bool hasDirections, bool isFixed): Component(owner) {
    this->isAnimated = true;
    this->numFrames = numFrames;
    this->animationSpeed = animationSpeed;
    this->isFixed = isFixed;
    
    if (hasDirections) {
        Animation downAnimation = Animation(0, numFrames, animationSpeed);
        Animation rightAnimation = Animation(1, numFrames, animationSpeed);
        Animation leftAnimation = Animation(2, numFrames, animationSpeed);
        Animation upAnimation = Animation(3, numFrames, animationSpeed);

        animations.emplace("DownAnimation", downAnimation);
        animations.emplace("RightAnimation", rightAnimation);
        animations.emplace("LeftAnimation", leftAnimation);
        animations.emplace("UpAnimation", upAnimation);

        this->animationIndex = 0;
        this->currentAnimationName = "DownAnimation";
    } else {
        Animation singleAnimation = Animation(0, numFrames, animationSpeed);
        animations.emplace("SingleAnimation", singleAnimation);
        this->animationIndex = 0;
        this->currentAnimationName = "SingleAnimation";
    }

    Play(this->currentAnimationName);

    SetTexture(assetTextureId);
}

void SpriteComponent::SetTexture(std::string assetTextureId) {
    /*std::cout << "SetTexture " << assetTextureId << std::endl;
    std::cout << " owner: " << owner << std::endl;
    std::cout << " owner level: " << owner->level->name << std::endl;*/
    texture = owner->level->assetManager->GetTexture(assetTextureId);
}

void SpriteComponent::Play(std::string animationName) {
    numFrames = animations[animationName].numFrames;
    animationIndex = animations[animationName].index;
    animationSpeed = animations[animationName].animationSpeed;
    currentAnimationName = animationName;
}

void SpriteComponent::Initialize() {
    transform = owner->GetComponent<TransformComponent>();
    sourceRectangle.x = 0;
    sourceRectangle.y = 0;
    sourceRectangle.w = transform->width;
    sourceRectangle.h = transform->height;
}

void SpriteComponent::Update(float deltaTime) {
    if (isAnimated) {
        sourceRectangle.x = sourceRectangle.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames);
    }
    sourceRectangle.y = animationIndex * transform->height;

    destinationRectangle.x = static_cast<int>(transform->position.x) - (isFixed ? 0 : owner->level->camera.x);
    destinationRectangle.y = static_cast<int>(transform->position.y) - (isFixed ? 0 : owner->level->camera.y);
    destinationRectangle.w = transform->width * transform->scale;
    destinationRectangle.h = transform->height * transform->scale;
}

void SpriteComponent::Render() {
    TextureManager::Draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
}
