#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../lib/lua/sol.hpp"

class LevelManager;

class Game {
    private:
        bool isRunning;
        SDL_Window *window;

    public:
        int ticksLastFrame;

        static SDL_Renderer *renderer;
        static SDL_Event event;
        LevelManager* levelManager;

        Game();
        ~Game();
        bool IsRunning() const;
        void LoadLevel(int levelNumber);
        void Initialize(int width, int height);
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
        void ProcessGameOver();
        void ProcessNextLevel(int levelNumber);
};

#endif