#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component
{
public:
    Entity* owner;
    Component(Entity* owner): owner(owner) {}
    virtual ~Component() {}
    virtual void Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
};

#endif