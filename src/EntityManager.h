#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "./Constants.h"
#include "./Entity.h"
#include "./Component.h"

#include <vector>

class Level;

class EntityManager
{
private:
    std::vector<Entity*> entities;

public:
    void ClearData();
    void Update(float deltaTime);
    void Render();
    bool HasNoEntities();
    Entity* AddEntity(std::string entityName, LayerType layer, Level* level);
    std::vector<Entity*> GetEntities() const;
    std::vector<Entity*> GetEntitiesByLayer(LayerType layer) const;
    Entity* GetEntityByName(std::string entityName) const;
    unsigned int GetEntityCount();
    //std::string CheckEntityCollisions(Entity& entity) const;
    CollisionType CheckCollisions() const;
    void DestroyInactiveEntities();
};

#endif