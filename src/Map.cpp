#include "./Map.h"

#include <fstream>

#include "./Game.h"
#include "./EntityManager.h"
#include "./Components/TileComponent.h"
#include "./Level.h"

Map::Map(std::string textureId, int scale, int tileSize) {
    this->textureId = textureId;
    this->scale = scale;
    this->tileSize = tileSize;
}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY, Level* level) {
    std::fstream mapFile;
    mapFile.open(filePath);

    for (int y = 0; y < mapSizeY; y++) {
        for (int x = 0; x < mapSizeX; x++) {
            char ch;
            mapFile.get(ch);
            int sourceRectY = atoi(&ch) * tileSize;
            mapFile.get(ch);
            int sourceRectX = atoi(&ch) * tileSize;

            AddTile(sourceRectX, sourceRectY, x * (scale * tileSize), y * (scale * tileSize), level);
            mapFile.ignore();
        }
    }

    mapFile.close();
}

void Map::AddTile(int sourceRectX, int sourceRectY, int x, int y, Level* level) {
    Entity* newTile(level->entityManager->AddEntity("Tile", LayerType::TILEMAP_LAYER, level));
    newTile->AddComponent<TileComponent>(newTile, sourceRectX, sourceRectY, x, y, tileSize, scale, textureId);
}
