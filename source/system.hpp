#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "entity.hpp"
#include "component.hpp"

void CollisionSystem(ComponentArray <TransformComponent>& transforms,ComponentArray <CollisionComponent>& collisions);
void MovementSystem(ComponentArray <TransformComponent>& transforms,ComponentArray <GravityComponent>& gravities, float dt);

class RenderSystem
{
    public:
    RenderSystem(sf::RenderWindow& w);
    void render(const ComponentArray <RenderComponent>& renders,const ComponentArray <TransformComponent>& transforms, sf::RenderWindow& window);
    void getTextures();
    void updateZoom(float factor);
    private:
    std::vector<sf::Texture> textures;
    std::vector<sf::Sprite> sprites;
    Entity player = 0;
    sf::RenderWindow& window;
    sf::View view;
    float zoomScale = 5.f;
    float zoomSpeed = 0.15f;
};
class AISystem
{
    void CheckPath();
    void Move();
    private:
    std::vector <Entity> bots;
    std::vector <Entity> players;
};
class InputSystem
{
    public:
    void getInput();
    inline Entity GetPlayer()
    {
        return this->player;
    }
    inline void SetPlayer(Entity player)
    {
        this->player = player;
    }
    private:
    Entity player = 0;
};
class ActionSystem 
{
    void Act();
};