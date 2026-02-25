#pragma once
#include <SFML/Graphics.hpp>
#include "entity.hpp"
#include "component.hpp"
#include "system.hpp"

class Scene
{
    public:
    Scene(sf::RenderWindow& window);
    void Initialize();
    void CreateSystem(float size,size_t planets);
    void CreatePlanet(sf::Vector2f position,sf::Vector2f velocity,float size,float mass,size_t textureIndex);
    void CreateStar(sf::Vector2f position,float size,float mass,size_t textureIndex);
    void CreatePlayer(sf::Vector2f position,sf::Vector2f size,float mass,size_t textureIndex);
    void update(float dt);
    void render(sf::RenderWindow& window);
    EntityManager eManager;
    ComponentManager cManager;
    RenderSystem rSystem;
    InputSystem iSystem;
};