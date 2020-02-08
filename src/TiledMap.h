#ifndef TILEDMAP_H
#define TILEDMAP_H

#include <string>
#include <map>

#include "../lib/nlohman/json.hpp"

class Level;

class Tileset {
public:
    std::string textureId;
    nlohmann::json& json;

    Tileset(std::string textureId, nlohmann::json& json);

    std::pair<int, int> TileIdToRectXY(int id);
};

class TiledMap {
private:
    std::map<int, Tileset> tilesets;
    nlohmann::json json;
    int scale;
    int tileSize;

    Tileset& GetTilesetFromId(int id);
    void AddTile(int sourceX, int sourceY, int x, int y, std::string textureId, Level* level);

public:
    TiledMap(std::string filePath, int scale);

    void Load(Level* level);
};

#endif