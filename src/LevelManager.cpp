#include "./LevelManager.h"

#include <iostream>

#include "../lib/lua/sol.hpp"

LevelManager::LevelManager(Game& game): game(game) {
}

LevelManager::~LevelManager() {

}

void LevelManager::Load(std::string levelName) {
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    lua.script_file("./assets/scripts/Config.lua");

    sol::table config = lua["Config"];
    
    std::string levelFilePath = config["levels"][levelName]["file"];
    Level* level = new Level(game, levelName, levelFilePath);
    level->Load();
    levels.emplace(levelName, level);
}

void LevelManager::Unload(std::string levelName) {

}

void LevelManager::UnloadAll() {

}

void LevelManager::Update(float deltaTime) {
    for (auto& level: levels) {
        level.second->Update(deltaTime);
    }
}

void LevelManager::Render() {
    for (auto& level: levels) {
        level.second->Render();
    }
}