#include <cstdint>
#include <queue>
#include <cassert> 
#include "entity.hpp"

EntityManager::EntityManager()
{
	// Initialize the queue with all possible entity IDs
	for (Entity i = 0; i < MAX_ENTITIES; ++i)
	{
		mAvailableEntities.push(i);
	}
}
Entity EntityManager::CreateEntity()
{
    assert(!mAvailableEntities.empty() && "No available entities!");
    Entity entity = mAvailableEntities.front();
    mAvailableEntities.pop();
    return entity;
}
void EntityManager::DestroyEntity(Entity entity,ComponentManager& cManager)
{
    assert(entity < MAX_ENTITIES && "Entity out of range");
    mAvailableEntities.push(entity);
}