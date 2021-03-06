#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <typeinfo>

#include "./Constants.h"
#include "./Level.h"

class Component;
class EntityManager;

class Entity
{
private:
    EntityManager& manager;
    bool isActive;
    std::vector<Component*> components;
    std::map<const std::type_info*, Component*> componentTypeMap;

public:
    std::string name;
    LayerType layer;
    Level* level = NULL;

    Entity(EntityManager& manager, std::string name, LayerType layer, Level* level);

    void Update(float deltaTime);
    void Render();
    void Destroy();
    bool IsActive() const;

    template<typename T, typename... TArgs>
    T& AddComponent(TArgs&&... args) {
        T* newComponent(new T(std::forward<TArgs>(args)...));
        components.emplace_back(newComponent);
        componentTypeMap[&typeid(*newComponent)] = newComponent;
        newComponent->Initialize();

        return *newComponent;
    }

    template<typename T>
    T* GetComponent() {
        return static_cast<T*>(componentTypeMap[&typeid(T)]);
    }

    template<typename T>
    bool HasComponent() const {
        auto it = componentTypeMap.find(&typeid(T));
        return it != componentTypeMap.end();
    }
};

#endif
