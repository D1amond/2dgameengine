#include "./Level.h"

#include <iostream>

#include "./Constants.h"
#include "./EntityManager.h"
#include "./Entity.h"
#include "./AssetManager.h"
#include "./Map.h"
#include "./TiledMap.h"

#include "./Components/ColliderComponent.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/ProjectileEmitterComponent.h"

Level::Level(Game& game, std::string name, std::string filePath): game(game) {
    this->name = name;
    this->filePath = filePath;
    camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    this->entityManager = new EntityManager();
    this->assetManager = new AssetManager(this->entityManager);
}

Level::~Level() {
    Unload();
}

void Level::Load() {
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    lua.script_file(filePath);

    sol::table data = lua[name];

    sol::table levelAssets = data["assets"];
    unsigned int assetIndex = 0;
    while (true) {
        sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
        if (existsAssetIndexNode == sol::nullopt) {
            break;
        } else {
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];
            if (assetType.compare("texture") == 0) {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                assetManager->AddTexture(assetId, assetFile.c_str());
            }
            if (assetType.compare("font") == 0) {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                int fontSize = asset["fontSize"];
                assetManager->AddFont(assetId, assetFile.c_str(), fontSize);
            }
        }
        assetIndex++;
    }

    sol::table levelMap = data["map"];
    std::string mapTextureId = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];

    map = new Map(
        mapTextureId,
        static_cast<int>(levelMap["scale"]),
        static_cast<int>(levelMap["tileSize"])
    );
    map->LoadMap(
        mapFile,
        static_cast<int>(levelMap["mapSizeX"]),
        static_cast<int>(levelMap["mapSizeY"]),
        this
    );

    sol::table levelEntities = data["entities"];
    unsigned int entityIndex = 0;
    while (true) {
        //std::cout << entityIndex << std::endl;
        sol::optional<sol::table> existsEntityIndexNode = levelEntities[entityIndex];
        if (existsEntityIndexNode == sol::nullopt) {
            break;
        }
        else {
            sol::table entity = levelEntities[entityIndex];
            std::string entityName = entity["name"];
            LayerType entityLayerType = static_cast<LayerType>(static_cast<int>(entity["layer"]));

            // Add new entity
            Entity* newEntity(entityManager->AddEntity(entityName, entityLayerType, this));

            // Add Transform Component
            sol::optional<sol::table> existsTransformComponent = entity["components"]["transform"];
            if (existsTransformComponent != sol::nullopt) {
                newEntity->AddComponent<TransformComponent>(
                    newEntity,
                    static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["velocity"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["velocity"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["width"]),
                    static_cast<int>(entity["components"]["transform"]["height"]),
                    static_cast<int>(entity["components"]["transform"]["scale"])
                );
            }

            // Add sprite component
            sol::optional<sol::table> existsSpriteComponent = entity["components"]["sprite"];
            if (existsSpriteComponent != sol::nullopt) {
                std::string textureId = entity["components"]["sprite"]["textureAssetId"];
                bool isAnimated = entity["components"]["sprite"]["animated"];
                if (isAnimated) {
                    newEntity->AddComponent<SpriteComponent>(
                        newEntity,
                        textureId,
                        static_cast<int>(entity["components"]["sprite"]["frameCount"]),
                        static_cast<int>(entity["components"]["sprite"]["animationSpeed"]),
                        static_cast<bool>(entity["components"]["sprite"]["hasDirections"]),
                        static_cast<bool>(entity["components"]["sprite"]["fixed"])
                    );
                } else {
                    newEntity->AddComponent<SpriteComponent>(newEntity, textureId, false);
                }
            }

            // Add input control component
            sol::optional<sol::table> existsInputComponent = entity["components"]["input"];
            if (existsInputComponent != sol::nullopt) {
                sol::optional<sol::table> existsKeyboardInputComponent = entity["components"]["input"]["keyboard"];
                if (existsKeyboardInputComponent != sol::nullopt) {
                    std::string upKey = entity["components"]["input"]["keyboard"]["up"];
                    std::string rightKey = entity["components"]["input"]["keyboard"]["right"];
                    std::string downKey = entity["components"]["input"]["keyboard"]["down"];
                    std::string leftKey = entity["components"]["input"]["keyboard"]["left"];
                    std::string shootKey = entity["components"]["input"]["keyboard"]["shoot"];
                    newEntity->AddComponent<KeyboardControlComponent>(newEntity, upKey, rightKey, downKey, leftKey, shootKey);
                }
            }

            // Add collider component
            sol::optional<sol::table> existsColliderComponent = entity["components"]["collider"];
            if (existsColliderComponent != sol::nullopt) {
                std::string colliderTag = entity["components"]["collider"]["tag"];
                newEntity->AddComponent<ColliderComponent>(
                    newEntity,
                    colliderTag,
                    static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["width"]),
                    static_cast<int>(entity["components"]["transform"]["height"])
                );
            }

            // Add projectile emitter component
            sol::optional<sol::table> existsProjectileEmitterComponent = entity["components"]["projectileEmitter"];
            if (existsProjectileEmitterComponent != sol::nullopt) {
                int parentEntityXPos = entity["components"]["transform"]["position"]["x"];
                int parentEntityYPos = entity["components"]["transform"]["position"]["y"];
                int parentEntityWidth = entity["components"]["transform"]["width"];
                int parentEntityHeight = entity["components"]["transform"]["height"];
                int projectileWidth = entity["components"]["projectileEmitter"]["width"];
                int projectileHeight = entity["components"]["projectileEmitter"]["height"];
                int projectileSpeed = entity["components"]["projectileEmitter"]["speed"];
                int projectileRange = entity["components"]["projectileEmitter"]["range"];
                int projectileAngle = entity["components"]["projectileEmitter"]["angle"];
                bool projectileShouldLoop = entity["components"]["projectileEmitter"]["shouldLoop"];
                std::string textureAssetId = entity["components"]["projectileEmitter"]["textureAssetId"];
                Entity* projectile(entityManager->AddEntity("projectile", PROJECTILE_LAYER, this));
                projectile->AddComponent<TransformComponent>(
                    projectile,
                    parentEntityXPos + (parentEntityWidth / 2),
                    parentEntityYPos + (parentEntityHeight / 2),
                    0,
                    0,
                    projectileWidth,
                    projectileHeight,
                    1
                );
                projectile->AddComponent<SpriteComponent>(projectile, textureAssetId);
                projectile->AddComponent<ProjectileEmitterComponent>(
                    projectile,
                    projectileSpeed,
                    projectileAngle,
                    projectileRange,
                    projectileShouldLoop
                );
                projectile->AddComponent<ColliderComponent>(
                    projectile,
                    "PROJECTILE",
                    parentEntityXPos,
                    parentEntityYPos,
                    projectileWidth,
                    projectileHeight
                );
            }
        }
        entityIndex++;
    }

    player = entityManager->GetEntityByName("player");
}

void Level::Unload() {
    entityManager->ClearData();
}

void Level::Update(float deltaTime) {
    entityManager->Update(deltaTime);

    HandleCameraMovement();
    CheckCollisions();
}

void Level::Render() {
    entityManager->Render();
}

void Level::HandleCameraMovement() {
    TransformComponent* playerTransform = player->GetComponent<TransformComponent>();

    camera.x = playerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = playerTransform->position.y - (WINDOW_HEIGHT / 2);

    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Level::CheckCollisions() {
    /*std::string collisionTagType = manager.CheckEntityCollisions(player);
    if (collisionTagType.compare("enemy") == 0) {
        isRunning = false;
    }*/

    CollisionType collisionType = entityManager->CheckCollisions();
    if (collisionType == CollisionType::PLAYER_ENEMY_COLLISION) {
        game.ProcessGameOver();
    }
    if (collisionType == CollisionType::PLAYER_PROJECTILE_COLLISION) {
        game.ProcessGameOver();
    }
    if (collisionType == CollisionType::PLAYER_LEVEL_COMPLETE_COLLISION) {
        game.ProcessNextLevel(1);
    }
}
