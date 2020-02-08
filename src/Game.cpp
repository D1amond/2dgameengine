#include <iostream>
#include <fstream>

#include "./Constants.h"
#include "./Game.h"
#include "./AssetManager.h"
#include "./Map.h"
#include "./TiledMap.h"
#include "../lib/glm/glm.hpp"
#include "../lib/nlohman/json.hpp"

#include "./Components/ColliderComponent.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/ProjectileEmitterComponent.h"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* map;
TiledMap* tiledMap;

Game::Game() {
    isRunning = false;
}

Game::~Game() {
}

bool Game::IsRunning() const {
    return isRunning;
}

void Game::Initialize(int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    if (TTF_Init() != 0) {
        std::cerr << "Error initializing SDL TTF." << std::endl;
    }

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS
    );

    if (!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    LoadLevel(0);

    isRunning = true;
    return;
}

Entity& player(manager.AddEntity("chopper", LayerType::PLAYER_LAYER));

void Game::LoadLevel(int levelNumber) {
    assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
    assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
    assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
    assetManager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
    assetManager->AddTexture("test-tiletexture", std::string("./assets/tilemaps/test.png").c_str());
    assetManager->AddTexture("projectile-image", std::string("./assets/images/bullet-enemy.png").c_str());
    assetManager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());

    assetManager->AddFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 14);

    /*tiledMap = new TiledMap("./assets/tilemaps/jungle.json", 2);
    tiledMap->Load();*/

    map = new Map("jungle-tiletexture", 2, 32);
    map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

    player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
    player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
    player.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");
    player.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);

    Entity& tankEntity(manager.AddEntity("tank", LayerType::ENEMIES_LAYER));
    tankEntity.AddComponent<TransformComponent>(150, 495, 5, 0, 32, 32, 1);
    tankEntity.AddComponent<SpriteComponent>("tank-image");
    tankEntity.AddComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32);

    Entity& projectileEntity(manager.AddEntity("projectile", LayerType::PROJECTILE_LAYER));
    projectileEntity.AddComponent<TransformComponent>(150+16, 495+16, 0, 0, 4, 4, 1);
    projectileEntity.AddComponent<SpriteComponent>("projectile-image");
    projectileEntity.AddComponent<ColliderComponent>("PROJECTILE", 150+16, 495+16, 4, 4);
    projectileEntity.AddComponent<ProjectileEmitterComponent>(50, 270, 200, true);

    Entity& heliportEntity(manager.AddEntity("heliport", LayerType::OBSTACLES_LAYER));
    heliportEntity.AddComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
    heliportEntity.AddComponent<SpriteComponent>("heliport-image");
    heliportEntity.AddComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32);

    Entity& radarEntity(manager.AddEntity("radar", LayerType::UI_LAYER));
    radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
    radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);

    Entity& labelLevelName(manager.AddEntity("LabelLevelName", LayerType::UI_LAYER));
    labelLevelName.AddComponent<TextLabelComponent>(10, 10, "First Level...", "charriot-font", WHITE_COLOR);
}

void Game::ProcessInput() {
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN: 
            if (event.key.keysym.sym == SDLK_ESCAPE) {  
                isRunning = false;
            }
            break;
        default:
            break;
    }
}

void Game::Update() {
    // Wait until 16ms has ellapsed since the last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));

    // Delta time is the difference in ticks from last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    // Clamp delta time to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass
    ticksLastFrame = SDL_GetTicks();

    manager.Update(deltaTime);

    HandleCameraMovement();
    CheckCollisions();
}

void Game::Render() {
    // set the background color
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    // clear the back buffer
    SDL_RenderClear(renderer);

    if (manager.HasNoEntities()) {
        return;
    }

    manager.Render();

    // swap front and back buffers
    SDL_RenderPresent(renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::HandleCameraMovement() {
    TransformComponent* playerTransform = player.GetComponent<TransformComponent>();

    camera.x = playerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = playerTransform->position.y - (WINDOW_HEIGHT / 2);

    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::CheckCollisions() {
    /*std::string collisionTagType = manager.CheckEntityCollisions(player);
    if (collisionTagType.compare("enemy") == 0) {
        isRunning = false;
    }*/

    CollisionType collisionType = manager.CheckCollisions();
    if (collisionType == CollisionType::PLAYER_ENEMY_COLLISION) {
        ProcessGameOver();
    }
    if (collisionType == CollisionType::PLAYER_PROJECTILE_COLLISION) {
        ProcessGameOver();
    }
    if (collisionType == CollisionType::PLAYER_LEVEL_COMPLETE_COLLISION) {
        ProcessNextLevel(1);
    }
}

void Game::ProcessGameOver() {
    std::cout << "Game Over" << std::endl;
    isRunning = false;
}

void Game::ProcessNextLevel(int levelNumber) {
    std::cout << "Next Level" << std::endl;
    isRunning = false;
}
