#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <string>
#include <unordered_map>

#include "./Game.h"
#include "./Level.h"

class LevelManager {
private:
    Game& game;
    std::unordered_map<std::string, Level*> levels;

public:
    LevelManager(Game& game);
    ~LevelManager();

    void Load(std::string levelName);
    void Unload(std::string levelName);
    void UnloadAll();

    void Update(float deltaTime);
    void Render();
};

#endif