#include "./AsepriteSpriteComponent.h"

#include <fstream>

#include "../Entity.h"
#include "../AssetManager.h"

AsepriteSpriteComponent::AsepriteSpriteComponent(Entity* owner, std::string filePath, std::string assetTextureId): Component(owner) {
    std::ifstream i(filePath);
    i >> json;
    i.close();

    texture = owner->level->assetManager->GetTexture(assetTextureId);
}

void AsepriteSpriteComponent::Play(std::string tag) {

}

void AsepriteSpriteComponent::Initialize() {
    for (auto& jsonAnimation: json["meta"]["frameTags"]) {
        AsepriteAnimation animation{jsonAnimation};
        for (int i = jsonAnimation["from"]; i <= jsonAnimation["to"]; i++) {
            AsepriteFrame frame{json["frames"][i]};
            animation.AddFrame(frame);
        }
        animations.emplace(jsonAnimation["name"], animation);
    }
}

void AsepriteSpriteComponent::Update(float deltaTime) {

}

void AsepriteSpriteComponent::Render() {

}
