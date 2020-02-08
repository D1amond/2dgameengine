#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../lib/lua/sol.hpp"

#include "./Game.h"

class EntityManager;
class AssetManager;
class Entity;
class Map;
class TiledMap;

class Level {
private:
    void HandleCameraMovement();
    void CheckCollisions();

public:
    SDL_Rect camera;
    Game& game;
    std::string name;
    std::string filePath;
    EntityManager* entityManager = NULL;
    AssetManager* assetManager = NULL;

    Entity* player = NULL;
    Map* map;
    TiledMap* tiledMap;

    Level(Game& game, std::string name, std::string filePath);
    ~Level();

    void Load();
    void Unload();

    void Update(float deltaTime);
    void Render();
};

#endif