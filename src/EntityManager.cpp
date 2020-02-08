#include "./EntityManager.h"

#include "./Collision.h"
#include "./Level.h"
#include "./Components/ColliderComponent.h"

void EntityManager::ClearData() {
    for (auto& entity: entities) {
        entity->Destroy();
    }
}

bool EntityManager::HasNoEntities() {
    return entities.size() == 0;
}

void EntityManager::Update(float deltaTime) {
    for (auto& entity: entities) {
        entity->Update(deltaTime);
    }
    DestroyInactiveEntities();
}

void EntityManager::Render() {
    for (int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++) {
        for (auto& entity: GetEntitiesByLayer(static_cast<LayerType>(layerNumber))) {
            entity->Render();
        }
    }
}

Entity* EntityManager::AddEntity(std::string entityName, LayerType layer, Level* level) {
    Entity *entity = new Entity(*this, entityName, layer, level);
    entities.emplace_back(entity);
    return entity;
}

std::vector<Entity*> EntityManager::GetEntities() const {
    return entities;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
    std::vector<Entity*> selectedEntities;
    for (auto& entity: entities) {
        if (entity->layer == layer) {
            selectedEntities.emplace_back(entity);
        }
    }
    return selectedEntities;
}

Entity* EntityManager::GetEntityByName(std::string entityName) const {
    for (auto* entity: entities) {
        if (entity->name.compare(entityName) == 0) {
            return entity;
        }
    }
    return NULL;
}

unsigned int EntityManager::GetEntityCount() {
    return entities.size();
}

/*std::string EntityManager::CheckEntityCollisions(Entity& entity) const {
    ColliderComponent* collider = entity.GetComponent<ColliderComponent>();
    for (auto& otherEntity: entities) {
        if (otherEntity->name.compare(entity.name) != 0 && otherEntity->name.compare("Tile") != 0) {
            if (otherEntity->HasComponent<ColliderComponent>()) {
                ColliderComponent* otherCollider = otherEntity->GetComponent<ColliderComponent>();
                if (Collision::CheckRectangleCollision(collider->collider, otherCollider->collider)) {
                    return otherCollider->colliderTag;
                }
            }
        }
    }
    return {};
}*/

CollisionType EntityManager::CheckCollisions() const {
    for (auto& thisEntity: entities) {
        if (thisEntity->HasComponent<ColliderComponent>()) {
            ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
            for (auto& thatEntity: entities) {
                if (thisEntity->name.compare(thatEntity->name) != 0 && thatEntity->HasComponent<ColliderComponent>()) {
                    ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
                    if (Collision::CheckRectangleCollision(thisCollider->collider, thatCollider->collider)) {
                        if (
                            thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("ENEMY") == 0
                        ) {
                            return CollisionType::PLAYER_ENEMY_COLLISION;
                        }
                        if (
                            thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("PROJECTILE") == 0
                        ) {
                            return CollisionType::PLAYER_PROJECTILE_COLLISION;
                        }
                        if (
                            thisCollider->colliderTag.compare("ENEMY") == 0 &&
                            thatCollider->colliderTag.compare("FRIENDLY_PROJECTILE") == 0
                        ) {
                            return CollisionType::ENEMY_PROJECTILE_COLLISION;
                        }
                        if (
                            thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("LEVEL_COMPLETE") == 0
                        ) {
                            return CollisionType::PLAYER_LEVEL_COMPLETE_COLLISION;
                        }
                    }
                }
            }
        }
    }
    return CollisionType::NO_COLLISION;
}

void EntityManager::DestroyInactiveEntities() {
    for (int i = 0; i < entities.size(); i++) {
        if (!entities[i]->IsActive()) {
            entities.erase(entities.begin() + i);
        }
    }
}
