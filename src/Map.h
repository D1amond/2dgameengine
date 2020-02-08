#ifndef MAP_H
#define MAP_H

#include <string>

class Level;

class Map {
private:
    std::string textureId;
    int scale;
    int tileSize;

public:
    Map(std::string textureId, int scale, int tileSize);
    ~Map();

    void LoadMap(std::string filePath, int mapSizeX, int mapSizeY, Level* level);
    void AddTile(int sourceX, int sourceY, int x, int y, Level* level);
};

#endif