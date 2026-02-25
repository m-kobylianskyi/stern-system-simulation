#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cassert>
#include <SFML/Graphics.hpp>
#include "entity.hpp"
#include "component.hpp"
#include "system.hpp"
#include "scene.hpp"
void CollisionSystem(ComponentArray <TransformComponent>& transforms,ComponentArray <CollisionComponent>& collisions)
{
    for(size_t i = 0;i<collisions.getSize()-1;i++)
    {
    Entity e1 = collisions.getEntity(i);
    TransformComponent& t1 = transforms.getData(e1);
    CollisionComponent& c1 = collisions.getData(e1);
        if(c1.type == CollisionType::Dynamic)
        {
            for(size_t j = 0;j<collisions.getSize();j++)
            {
                Entity e2 = collisions.getEntity(j);
                TransformComponent& t2 = transforms.getData(e2);
                CollisionComponent& c2 = collisions.getData(e2);
                //CheckCollision
                //MakeChanges
            }
        }
    }
}
void MovementSystem(ComponentArray <TransformComponent>& transforms,ComponentArray <GravityComponent>& gravities, float dt)
{ 
    const float G = 6.67430e-2f;
    dt = dt*100.0f;
    for(size_t i = 0;i<gravities.getSize()-1;i++)
    {
        Entity e1 = gravities.getEntity(i);
        GravityComponent& g1 = gravities.getData(e1);
        TransformComponent& t1 = transforms.getData(e1);
        for(size_t j = i+1;j<gravities.getSize();j++)
        {
            Entity e2 = gravities.getEntity(j);
            GravityComponent& g2 = gravities.getData(e2);
            TransformComponent& t2 = transforms.getData(e2);
            sf::Vector2f dir = t2.position-t1.position;
            float dist2 = std::max((dir.x*dir.x + dir.y*dir.y), 0.01f);
            sf::Vector2f force = (G * g1.mass * g2.mass / dist2) * (dir / std::sqrt(dist2));
            t1.velocity += force / g1.mass * dt;
            t2.velocity -= force / g2.mass * dt;
        }
    }
    for(size_t i = 0;i<transforms.getSize();i++)
    {
        Entity e = transforms.getEntity(i);
        TransformComponent& t = transforms.getData(e);
        t.position += t.velocity * dt;
        //t.rotation += 1.0f * dt;
    }
}
RenderSystem::RenderSystem(sf::RenderWindow& w) : window(w){
    view = window.getDefaultView();
    getTextures();
}
void RenderSystem::updateZoom(float factor)
{
    zoomScale *= factor;

    zoomScale = std::clamp(zoomScale, 0.3f, 10.0f);
}
void RenderSystem::render(const ComponentArray <RenderComponent>& renders,const ComponentArray <TransformComponent>& transforms, sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    sf::Texture bgTexture("../assets/textures/background.png");
    sf::Sprite background(bgTexture);
    background.setScale({4.0f, 4.0f});
    window.draw(background);
    auto camera = transforms.getData(player);
    view.setCenter(camera.position);
    view.setSize({2560.0f, 1600.0f});
    view.zoom(zoomScale); 
    window.setView(view);
    for(size_t i = 0;i<renders.getSize();i++)
    {
        Entity e = renders.getEntity(i);
        const auto& t = transforms.getData(e);
        const auto& r = renders.getData(e);
        sf::Sprite sprite(textures[r.textureIndex]);
        auto texSize = sprite.getTexture().getSize();
        float scaleX = t.size.x / texSize.x;
        float scaleY = t.size.y / texSize.y;
        sprite.setScale({scaleX, scaleY});
        sprite.setOrigin({(texSize.x / 2.0f), (texSize.y / 2.0f)});
        sprite.setPosition(t.position);
        sprite.setRotation(sf::degrees(t.rotation));
        window.draw(sprite);
    }
}
void RenderSystem::getTextures(){
    std::ifstream textureslist("../assets/textures/list.txt");
    if(!textureslist.is_open()) 
    {
        return;
    }
    std::string line;
    while(getline(textureslist,line))
    {
        sf::Texture tempT(line);
        textures.push_back(std::move(tempT));
    }
}