#include "./TiledMap.h"

#include <fstream>

#include "./EntityManager.h"
#include "./Components/TileComponent.h"

extern EntityManager manager;

Tileset::Tileset(std::string textureId, nlohmann::json& json): textureId(textureId), json(json) {

}

/**
 * Transform a tile id to the source rectangle x and y coordinates in the tileset texture 
 */
std::pair<int, int> Tileset::TileIdToRectXY(int id) {

}

TiledMap::TiledMap(std::string filePath, int scale) {
    this->scale = scale;

    std::ifstream i(filePath);
    i >> json;
    i.close();

    this->tileSize = json["tileheight"];
}

void TiledMap::Load() {
    // Get tilesets
    int id = 0;
    for (auto& jsonTileset: json["tilesets"]) {
        Tileset tileset{jsonTileset["name"], jsonTileset};
        tilesets.emplace(id, tileset);
        id++;
    }

    // Get tiles by layer
    for (auto& jsonLayer: json["layers"]) {
        if (jsonLayer["type"] == "tilelayer") {
            int processedTileCount = 0;
            for (int tileId: jsonLayer["data"]) {


                processedTileCount++;
            }
        }
    }
}

void TiledMap::AddTile(int sourceRectX, int sourceRectY, int x, int y) {
    Entity& newTile(manager.AddEntity("Tile", LayerType::TILEMAP_LAYER));
    newTile.AddComponent<TileComponent>(sourceRectX, sourceRectY, x, y, tileSize, scale, textureId);
}