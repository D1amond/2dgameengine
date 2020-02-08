#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../Game.h"
#include "../AssetManager.h"

class TextLabelComponent: public Component {
private:
    SDL_Rect position;
    std::string text;
    std::string fontFamily;
    SDL_Color color;
    SDL_Texture* texture;

public:
    TextLabelComponent(int x, int y, std::string text, std::string fontFamily, const SDL_Color color);

    void SetLabelText();

    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;
};

#endif