#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "../Animation.h"
#include "./TransformComponent.h"

class SpriteComponent: public Component
{
private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    bool isAnimated;
    int numFrames;
    int animationSpeed;
    bool isFixed;
    std::map<std::string, Animation> animations;
    std::string currentAnimationName;
    unsigned int animationIndex = 0;


public:
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent(std::string assetTextureId);
    SpriteComponent(std::string assetTextureId, bool isFixed);
    SpriteComponent(std::string assetTextureId, int numFrames, int animationSpeed, bool hasDirections, bool isFixed);
    void SetTexture(std::string assetTextureId);
    void Play(std::string animationName);
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;
};

#endif