#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <string>
#include <SDL2/SDL.h>
#include "../../lib/glm/glm.hpp"

#include "../EntityManager.h"

class TileComponent: public Component {
public:
    SDL_Texture *texture;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    glm::vec2 position;

    TileComponent(Entity* owner, int sourceRectX, int sourceRectY, int x, int y, int tileSize, int tileScale, std::string assetTextureId);
    ~TileComponent();

    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;
};

#endif