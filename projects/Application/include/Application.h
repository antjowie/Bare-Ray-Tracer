#pragma once
#include "Scene.h"
#include "Camera.h"
#include "RayTracer.h"

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>


class Application
{
public:
	Application();

	void run();

private:
	struct Configuration
	{
	public:
		int fps;
		int imageWidth;
		int imageHeight;
		int windowWidth;
		int windowHeight;
		float maxRenderTime;
	};
	Configuration m_config;
	sf::Image m_image;
	
	SceneLoader m_sceneLoader;
	Scene *m_scene;
	Camera *m_camera;
	RayTracer m_rayTracer;

	int m_currentScene;

	bool loadConfiguration();
	void pollEvents(sf::RenderWindow &window, sf::Event &event);
	void transformCamera(const float elapsedTime);
};