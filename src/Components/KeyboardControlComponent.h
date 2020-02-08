#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"
#include "./SpriteComponent.h"

class KeyboardControlComponent: public Component {
public:
	std::string upKey;
	std::string downKey;
	std::string rightKey;
	std::string leftKey;
    std::string shootKey;

    TransformComponent *transform;
    SpriteComponent *sprite;

    KeyboardControlComponent(Entity* owner, std::string upKey, std::string rightKey, std::string downKey, std::string leftKey, std::string shootKey);

    std::string GetSDLKeyStringCode(std::string key);

    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;
};

#endif