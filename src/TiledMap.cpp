#include "./TiledMap.h"

#include <cmath>
#include <fstream>
#include <iostream>

#include "./EntityManager.h"
#include "./Components/TileComponent.h"

extern EntityManager manager;

Tileset::Tileset(std::string textureId, nlohmann::json& json): textureId(textureId), json(json) {}

/**
 * Transform a tile id to the source rectangle x and y coordinates in the tileset texture 
 */
std::pair<int, int> Tileset::TileIdToRectXY(int id) {
    id =  id - static_cast<int>(json["firstgid"]);

    int tileSize = json["tileheight"];
    int imgW = json["imagewidth"];
    int imgH = json["imageheight"];
    int w = imgW / tileSize;
    int h = imgH / tileSize;

    //std::cout << "sourceX = ((" << id << " - 1) % " << w << ") * " << tileSize << std::endl;
    //std::cout << "sourceY = floor((" << id << " - 1) / " << w << ") * " << tileSize << std::endl;

    int sourceX = ((id) % w) * tileSize;
    int sourceY = std::floor((id) / w) * tileSize;

    return {sourceX, sourceY};
}

Tileset& TiledMap::GetTilesetFromId(int id) {
    for (auto& tileset: tilesets) {
        if (tilesets.count(tileset.first + 1)) {
            auto& nextTileset = tilesets.at(tileset.first + 1);
            if (id < nextTileset.json["firstgid"]) {
                return tileset.second;
            }
        } else {
            if (id > tileset.second.json["firstgid"]) {
                return tileset.second;
            }
        }
    }
    return tilesets.at(0);
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
    //std::cout << "nb tileset: " << tilesets.size() << std::endl;

    // Get tiles by layer
    for (auto& jsonLayer: json["layers"]) {
        if (jsonLayer["type"] == "tilelayer") {
            int processedTileCount = 0;
            int x = 0;
            int y = 0;
            for (int tileId: jsonLayer["data"]) {
                if (x == (jsonLayer["width"])) {
                    y++;
                    x = 0;
                }

                if (tileId > 0) {
                    auto& tileset = GetTilesetFromId(tileId);
                    //std::cout << "tile id " << tileId << " in tileset " << tileset.textureId << std::endl;

                    std::pair<int, int> sourceRect = tileset.TileIdToRectXY(tileId);
                    AddTile(sourceRect.first, sourceRect.second, x * (tileSize * scale), y * (tileSize * scale), tileset.textureId);
                }
                
                x++;
                processedTileCount++;
            }
        }
    }
}

void TiledMap::AddTile(int sourceRectX, int sourceRectY, int x, int y, std::string textureId) {
    //std::cout << "source = (" << sourceRectX << "," << sourceRectY << "), coord = (" << x << "," << y << "), texture = " << textureId << std::endl;
    Entity& newTile(manager.AddEntity("Tile", LayerType::TILEMAP_LAYER));
    newTile.AddComponent<TileComponent>(sourceRectX, sourceRectY, x, y, tileSize, scale, textureId);
}