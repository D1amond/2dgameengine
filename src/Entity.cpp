#include "./Entity.h"

#include "./EntityManager.h"

Entity::Entity(EntityManager& manager, std::string name, LayerType layer, Level* level): manager(manager), name(name), layer(layer), level(level) {
    isActive = true;
}

void Entity::Update(float deltaTime) {
    for (auto& component: components) {
        component->Update(deltaTime);
    }
}

void Entity::Render() {
    for (auto& component: components) {
        component->Render();
    }
}

void Entity::Destroy() {
    isActive = false;
}

bool Entity::IsActive() const {
    return isActive;
}
