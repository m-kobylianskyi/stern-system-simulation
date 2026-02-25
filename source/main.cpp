#include <SFML/Graphics.hpp>
#include <time.h>
#include "scene.hpp"
class Simulation 
{
	public:
	Simulation() : currentScene(window)
	{
		window.create(sf::VideoMode({3840, 1920}), "Into the Stars!!!");
	}
	void run()
	{
		srand(time(NULL));
		
		currentScene.Initialize();

		sf::Clock clock;
		while(window.isOpen())
		{
			float dt = clock.restart().asSeconds();
			while(const std::optional event = window.pollEvent())
			{
				if(event->is<sf::Event::Closed>())
				{
					window.close();
				}
				if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()){
					float factor = (mouseWheelScrolled->delta > 0) ? 0.92f : 1.08f;
					currentScene.rSystem.updateZoom(factor);
				}
			}
			currentScene.update(dt);
			window.clear();
			currentScene.render(window);
			window.display();
		}
	}
	private:
	sf::RenderWindow window;
	Scene currentScene;
};

int main()
{
	Simulation simulation;
	simulation.run();
	return 0;
	
}