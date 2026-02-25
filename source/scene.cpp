#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "entity.hpp"
#include "component.hpp"
#include "system.hpp"
#include "scene.hpp"

Scene::Scene(sf::RenderWindow& window) : rSystem(window)
{
    cManager.RegisterComponent<TransformComponent>();
    cManager.RegisterComponent<CollisionComponent>();
    cManager.RegisterComponent<HealthComponent>();
    cManager.RegisterComponent<StatsComponent>();
    cManager.RegisterComponent<InventoryComponent>();
    cManager.RegisterComponent<RenderComponent>();
    cManager.RegisterComponent<GravityComponent>();
    cManager.RegisterComponent<ActionComponent>();

    std::cout<<"Textures are loaded and Components added"<<std::endl;
}
void Scene::Initialize()
{
    this->CreateSystem(3840.0f,0);
}
//First cosmic speed for planets
sf::Vector2f FCS(float mass,sf::Vector2f pos1,sf::Vector2f pos2)
{
    const float G = 6.67430e-2f;
    sf::Vector2f dir = pos2 - pos1;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    dist = std::max(dist, 0.01f);
    sf::Vector2f normDir = dir / dist;
    float speed = std::sqrt((G * mass) / dist);
    sf::Vector2f tangential(-normDir.y, normDir.x);
    return tangential * speed;
}
void Scene::CreateSystem(float size,size_t planets = 0)
{
    if(planets == 0){
        planets = rand() % 12 + 3;
    }
    int starType = rand() % 100;
    float starMass = 0.0f;
    float starSize = 0.0f;
    int starTextureIndex = 0;
    if(starType<75)
    {
        starMass = (rand() % 300 + 800) * 100.0f;
        starSize = (rand() % 600 + 100) * 10.0f;
        starTextureIndex = rand() % 4;
    }else if(starType<85)
    {
        starMass = (rand() % 1300 + 900) * 100.0f;
        starSize = (rand() % 1200 + 800) * 10.0f;
        starTextureIndex = rand() % 9 + 5;
    }else if(starType<95)
    {
        starMass = (rand() % 2500 + 1500) * 100.0f;
        starSize = (rand() % 3000 + 1200) * 10.0f;
        starTextureIndex = rand() % 13 + 10;
    }else
    {
        starMass = (rand() % 6000 + 3000) * 100.0f;
        starSize = (rand() % 1800 + 300) * 10.0f;
        starTextureIndex = rand() % 17 + 14;
    }
    sf::Vector2f starPos = {0.0f,0.0f};
    this->CreateStar(starPos,starSize,starMass,starTextureIndex);
    float distance = 200.0f + starSize/2;
    for(size_t i = 0;i<planets;i++)
    {
        int planetType = rand() % 100;
        float planetMass = 0.0f;
        float planetSize = 0.0f;
        int planetTextureIndex = 0;
        if(planetType<35)
        {
            planetMass = (rand() % 30 + 10) * 1.0f;
            planetSize = (rand() % 120 + 30) * 1.0f;
            planetTextureIndex = rand() % 25 + 18;
        }else if(planetType<60)
        {
            planetMass = (rand() % 120 + 30) * 1.0f;
            planetSize = (rand() % 250 + 120) * 1.0f;
            planetTextureIndex = rand() % 25 + 18;
        }else if(planetType<80)
        {
            planetMass = (rand() % 300 + 80) * 1.0f;
            planetSize = (rand() % 200 + 400) * 1.0f;
            planetTextureIndex = rand() % 25 + 18;
        }else if(planetType<90)
        {
            planetMass = (rand() % 1500 + 300) * 1.0f;
            planetSize = (rand() % 600 + 350) * 1.0f;
            planetTextureIndex = rand() % 25 + 18;
        }else
        {
            planetMass = (rand() % 3000 + 500) * 1.0f;
            planetSize = (rand() % 1400 + 800) * 1.0f;
            planetTextureIndex = rand() % 25 + 18;
        }
        distance += planetSize/2;
        sf::Vector2f planetPos = {starPos.x - distance,starPos.y};
        distance *= 1.3; 
        sf::Vector2f planetSpeed = FCS(starMass,planetPos,starPos);
        this->CreatePlanet(planetPos,planetSpeed,planetSize,planetMass,planetTextureIndex);
    }
}
void Scene::CreatePlanet(sf::Vector2f position,sf::Vector2f velocity,float size,float mass,size_t textureIndex)
{
    Entity e = eManager.CreateEntity();
    TransformComponent t = {position,velocity,{size,size},0.0f};
    CollisionComponent c = {CollisionType::Static};
    GravityComponent g = {mass};
    RenderComponent r = {1,textureIndex};
    cManager.AddComponent(e,t);
    cManager.AddComponent(e,c);
    cManager.AddComponent(e,g);
    cManager.AddComponent(e,r);
    std::cout << "Creating PLANET: entity=" << e 
              << " pos=(" << position.x << ", " << position.y << ")"
              << " size=" << size << " texIdx=" << textureIndex << "\n";
    
}
void Scene::CreateStar(sf::Vector2f position,float size,float mass,size_t textureIndex)
{
    Entity e = eManager.CreateEntity();
    TransformComponent t = {position,{0.0f,0.0f},{size,size},0.0f};
    CollisionComponent c = {CollisionType::Static};
    GravityComponent g = {mass};
    RenderComponent r = {1,textureIndex};
    cManager.AddComponent(e,t);
    cManager.AddComponent(e,c);
    cManager.AddComponent(e,g);
    cManager.AddComponent(e,r);
    std::cout << "Creating STAR: entity=" << e 
              << " pos=(" << position.x << ", " << position.y << ")"
              << " size=" << size << " texIdx=" << textureIndex << "\n";
}
void Scene::CreatePlayer(sf::Vector2f position,sf::Vector2f size,float mass,size_t textureIndex){
    Entity e = eManager.CreateEntity();
    TransformComponent t = {position,{0.0f,0.0f},size,0.0f};
    CollisionComponent c = {CollisionType::Static};
    GravityComponent g = {mass};
    RenderComponent r = {1,textureIndex};
    cManager.AddComponent(e,t);
    cManager.AddComponent(e,c);
    cManager.AddComponent(e,g);
    cManager.AddComponent(e,r);
    iSystem.SetPlayer(e);
}
void Scene::update(float dt){
    MovementSystem(*cManager.GetComponentArray<TransformComponent>(),*cManager.GetComponentArray<GravityComponent>(),dt);
}
void Scene::render(sf::RenderWindow& window){
    rSystem.render(*cManager.GetComponentArray<RenderComponent>(),*cManager.GetComponentArray<TransformComponent>(),window);
}