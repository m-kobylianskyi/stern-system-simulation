#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <memory>
#include <cassert>
#include <SFML/Graphics.hpp>
#include "entity.hpp"

using ComponentType = std::size_t;

enum CollisionType{
    Static,
    Dynamic
};
enum ActionType {
    Move_Forward,
    Move_Backward,
    Move_Left,
    Move_Right,
    Fire
};
struct TransformComponent{
    sf::Vector2f position = {0.0f,0.0f};
    sf::Vector2f velocity = {0.0f,0.0f};
    sf::Vector2f size = {0.0f,0.0f};
    float rotation = 0.0f;
};
struct CollisionComponent{
    CollisionType type = CollisionType::Static;
};
struct HealthComponent{
    float health = 100.0f;
};
struct StatsComponent {
    int attack = 0;
    int attackSpeed = 0;
    int defense = 0;
    int speed = 0;
};
struct InventoryComponent {
    int metals = 0;
    int minerals = 0;
    int plasma = 0;
};
struct RenderComponent {
    int layer = 0;
    size_t textureIndex;
};
struct GravityComponent {
    float mass = 0;
};
struct ActionComponent {
    ActionType type;
    float force;
};
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};
template<typename T>
class ComponentArray : public IComponentArray
{
    public:
    void InsertData(Entity entity, T component)
    {
        //Check if Entity is valid
        assert(entity < MAX_ENTITIES && "Entity out of range");
        //Check if Component exist
        assert(sparse[entity] == -1 && "Component already exists");
        //Bind Component to Enitity
        sparse[entity] = dense.size();
        denseEntities.push_back(entity);
        //Add Component to Array
        dense.push_back(component);
    }
    void RemoveData(Entity entity)
    {
        //Check if component exist
        assert(sparse[entity] != -1 && "Component does not exist");
        //Swap Component and its Entity if needed
        size_t index = sparse[entity];
        if(sparse[entity] != dense.size()-1){
            std::swap(dense.back(),dense.at(index));
            std::swap(denseEntities.back(),denseEntities.at(index));
            //Update sparse for moved Entity
            sparse[denseEntities[index]] = index;
        }
        //Delete Component And its Entity
        dense.pop_back();
        denseEntities.pop_back();
        //Set that Entity has this Component no longer
        sparse[entity] = -1;
    }
    void EntityDestroyed(Entity entity)
	{
        assert(entity < MAX_ENTITIES && "Entity out of range");
		if (sparse[entity] != -1)
		{
			// Remove the entity's component if it existed
			RemoveData(entity);
		}
	}
    inline Entity getEntity(size_t ComponentIndex) const {
        return denseEntities[ComponentIndex];
    }
    const T& getData(Entity entity) const
	{
        //Check if Component exist
        assert(sparse[entity] != -1 && "Component does not exist");
        //Return Component
        return dense[sparse[entity]];
	}
    T& getData(Entity entity)
	{
        //Check if Component exist
        assert(sparse[entity] != -1 && "Component does not exist");
        //Return Component
        return dense[sparse[entity]];
	}
    inline size_t getSize() const {
        return dense.size();
    }
    
    private:
    //Entity to ComponentIndex
    std::vector<int> sparse = std::vector<int>(MAX_ENTITIES, -1); ;
    //ComponentIndex to Entity
    std::vector<Entity> denseEntities;
    //Component Array
    std::vector<T> dense;
};

class ComponentManager
{
    public:
    template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

		// Add this component type to the component type map
		mComponentTypes.insert({typeName, mNextComponentType});

		// Create a ComponentArray pointer and add it to the component arrays map
		mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

		// Increment the value so that the next component registered will be different
		++mNextComponentType;
	}
    template<typename T>
    void AddComponent(Entity entity, T component){
        GetComponentArray<T>()->InsertData(entity, component);
    }
    template<typename T>
	void RemoveComponent(Entity entity)
	{
		// Remove a component from the array for an entity
		GetComponentArray<T>()->RemoveData(entity);
	}
    template<typename T>
	T& GetComponent(Entity entity)
	{
		// Get a reference to a component from the array for an entity
		return GetComponentArray<T>()->GetData(entity);
	}
    void EntityDestroyed(Entity entity){
        for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
    }
    // Convenience function to get the statically casted pointer to the ComponentArray of type T.
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		std::string typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}
    private:
    // Map from type string pointer to a component type
	std::unordered_map<std::string, ComponentType> mComponentTypes{};

	// Map from type string pointer to a component array
	std::unordered_map<std::string, std::shared_ptr<IComponentArray>> mComponentArrays{};

	// The component type to be assigned to the next registered component - starting at 0
	ComponentType mNextComponentType{};
};