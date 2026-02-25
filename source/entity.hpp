#pragma once
#include <cstdint>
#include <queue>

class ComponentManager;

constexpr std::size_t MAX_ENTITIES = 5000;
using Entity = uint32_t;

class EntityManager 
{
    public:
    EntityManager();
    Entity CreateEntity();
    void DestroyEntity(Entity entity,ComponentManager& cManager);
    private:
    std::queue<Entity> mAvailableEntities{};
};