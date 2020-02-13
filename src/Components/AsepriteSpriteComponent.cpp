#include "./AsepriteSpriteComponent.h"

#include <fstream>

#include "../Entity.h"
#include "../AssetManager.h"
#include "../TextureManager.h"

#include "./TransformComponent.h"

AsepriteSpriteComponent::AsepriteSpriteComponent(Entity* owner, std::string filePath, std::string assetTextureId): Component(owner) {
    std::ifstream i(filePath);
    i >> json;
    i.close();

    texture = owner->level->assetManager->GetTexture(assetTextureId);
}

AsepriteSpriteComponent::AsepriteSpriteComponent(Entity* owner, std::string filePath, std::string assetTextureId, bool fixed): Component(owner) {
    std::ifstream i(filePath);
    i >> json;
    i.close();

    texture = owner->level->assetManager->GetTexture(assetTextureId);
    this->fixed = fixed;
}

void AsepriteSpriteComponent::Play(std::string tag) {

    selectedTag = tag;
}

void AsepriteSpriteComponent::Initialize() {
    for (auto& jsonAnimation: json["meta"]["frameTags"]) {
        AsepriteAnimation animation{jsonAnimation};
        for (int i = jsonAnimation["from"]; i <= jsonAnimation["to"]; i++) {
            AsepriteFrame frame{i, json["frames"][i]};
            animation.AddFrame(frame);
        }
        animations.emplace(jsonAnimation["name"], animation);
        if (selectedTag == "") {
            selectedTag = jsonAnimation["name"];
        }
    }

    transform = owner->GetComponent<TransformComponent>();
    sourceRectangle.x = 0;
    sourceRectangle.y = 0;
    sourceRectangle.w = transform->width;
    sourceRectangle.h = transform->height;
}

void AsepriteSpriteComponent::Update(float deltaTime) {
    animations[selectedTag].Update(deltaTime);

    sourceRectangle.x = animations[selectedTag].currentFrame.x;
    sourceRectangle.y = animations[selectedTag].currentFrame.y;
    sourceRectangle.w = animations[selectedTag].currentFrame.w;
    sourceRectangle.h = animations[selectedTag].currentFrame.h;

    destinationRectangle.x = static_cast<int>(transform->position.x) - (fixed ? 0 : owner->level->camera.x);
    destinationRectangle.y = static_cast<int>(transform->position.y) - (fixed ? 0 : owner->level->camera.y);
    destinationRectangle.w = transform->width * transform->scale;
    destinationRectangle.h = transform->height * transform->scale;
}

void AsepriteSpriteComponent::Render() {
    TextureManager::Draw(texture, sourceRectangle, destinationRectangle, animations[selectedTag].flip);
}
