#ifndef ASEPRITESPRITECOMPONENT_H
#define ASEPRITESPRITECOMPONENT_H

#include <string>
#include <map>
#include <vector>

#include <SDL2/SDL.h>
#include "../../lib/nlohman/json.hpp"

#include "../Component.h"

class Entity;

class AsepriteFrame {
public:
    nlohmann::json json;

    AsepriteFrame(nlohmann::json json): json(json) {}
};

class AsepriteAnimation {
private:
    std::vector<AsepriteFrame> frames;

public:
    nlohmann::json json;

    AsepriteAnimation(nlohmann::json json): json(json) {}

    void AddFrame(AsepriteFrame frame) {
        frames.emplace_back(frame);
    }
};

class AsepriteSpriteComponent: public Component {
private:
    SDL_Texture* texture;
    nlohmann::json json;
    std::map<std::string, AsepriteAnimation> animations;

public:
    AsepriteSpriteComponent(Entity* owner, std::string filePath, std::string assetTextureId);

    void Play(std::string tag);

    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;
};

#endif