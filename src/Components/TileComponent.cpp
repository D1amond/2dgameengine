#include "./TileComponent.h"

#include "../AssetManager.h"

TileComponent::TileComponent(int sourceRectX, int sourceRectY, int x, int y, int tileSize, int tileScale, std::string assetTextureId) {
    texture = Game::assetManager->GetTexture(assetTextureId);

    sourceRectangle.x = sourceRectX;
    sourceRectangle.y = sourceRectY;
    sourceRectangle.w = tileSize;
    sourceRectangle.h = tileSize;

    destinationRectangle.x = x;
    destinationRectangle.y = y;
    destinationRectangle.w = tileSize * tileScale;
    destinationRectangle.h = tileSize * tileScale;

    position.x = x;
    position.y = y;
}

TileComponent::~TileComponent() {
    SDL_DestroyTexture(texture);
}

void TileComponent::Initialize() {

}

void TileComponent::Update(float deltaTime) {
    destinationRectangle.x = position.x - Game::camera.x;
    destinationRectangle.y = position.y - Game::camera.y;
}

void TileComponent::Render() {
    TextureManager::Draw(texture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE);
}
