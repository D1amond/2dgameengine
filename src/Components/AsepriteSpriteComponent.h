#ifndef ASEPRITESPRITECOMPONENT_H
#define ASEPRITESPRITECOMPONENT_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <utility>

#include <SDL2/SDL.h>
#include "../../lib/nlohman/json.hpp"

#include "../Component.h"

class Entity;
class TransformComponent;

class AsepriteFrame {
public:
    int id = 0;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    float duration = 0.0f;
    bool empty = true;

    AsepriteFrame() {}

    AsepriteFrame(int id, nlohmann::json json) {
        this->id = id;
        x = json["frame"]["x"];
        y = json["frame"]["y"];
        w = json["frame"]["w"];
        h = json["frame"]["h"];
        int sourceDuration = json["duration"];
        duration = static_cast<float>(sourceDuration) / 1000;
        empty = false;
    }
};

class AsepriteAnimation {
private:
    std::vector<AsepriteFrame> frames;

    float getDurationToFrame(float time) const {
        float totalDuration = 0.0f;
        for (auto& frame : frames) {
            if (totalDuration + frame.duration <= time) {
                totalDuration += frame.duration;
            } else {
                break;
            }
        }
        return totalDuration;
    }

    std::pair<AsepriteFrame, AsepriteFrame> clamp(float time) {
        auto&& value = std::find_if(frames.begin(), frames.end(), [&, totalDuration = 0.0](auto&& keyframe) mutable {
            totalDuration += keyframe.duration;
            return time < totalDuration;
        });
        
        if (value >= frames.end() - 1) {
            return {frames.back(), frames.back()};
        }
        
        auto value2 = value;
        value2++;
        return {*value, *value2};
    }

    std::pair<AsepriteFrame, AsepriteFrame> repeat(float time) {
        float animationDuration = std::accumulate(frames.begin(), frames.end(), 0.0, [](auto&& total, auto&& keyframe){
            return total + keyframe.duration;
        });
        
        float ajustedTime = std::fmod(time, animationDuration);
        
        auto&& value = std::find_if(frames.begin(), frames.end(), [&, totalDuration = 0.0](auto&& keyframe) mutable {
            totalDuration += keyframe.duration;
            return ajustedTime < totalDuration;
        });
        
        if (value >= frames.end() - 1) {
            return {frames.back(), frames.front()};
        }
        
        auto value2 = value;
        value2++;
        return {*value, *value2};
    }

    AsepriteFrame nearest(float time, AsepriteFrame first, AsepriteFrame second) {
        if (time >= first.duration) {
            return second;
        }
        return first;
    }

public:
    std::string name;
    int from = 0;
    int to = 0;
    float cursor = 0.0f;
    bool loop = true;
    float totalDuration = 0.0f;
    AsepriteFrame currentFrame;
    AsepriteFrame currentFrameInitial;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    AsepriteAnimation(nlohmann::json json) {
        name = json["name"];
        from = json["from"];
        to = json["to"];
    }

    void AddFrame(AsepriteFrame frame) {
        frames.emplace_back(frame);
        totalDuration += frame.duration;
        if (currentFrame.empty) {
            currentFrame = frame;
            currentFrameInitial = frame;
        }
    }

    void Reset() {
        currentFrame = currentFrameInitial;
        cursor = 0.0f;
    }

    void Update(float deltaTime) {
        cursor += deltaTime;
        auto value = repeat(cursor);
        auto _cursor = std::fmod(cursor, totalDuration);
        currentFrame = nearest(_cursor - getDurationToFrame(_cursor), value.first, value.second);
    }
};

class AsepriteSpriteComponent: public Component {
private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    nlohmann::json json;
    std::map<std::string, AsepriteAnimation> animations;
    std::string selectedTag = "";
    bool fixed = false;

public:
    AsepriteSpriteComponent(Entity* owner, std::string filePath, std::string assetTextureId);
    AsepriteSpriteComponent(Entity* owner, std::string filePath, std::string assetTextureId, bool fixed);

    void Play(std::string tag);

    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;
};

#endif