#include "Application.h"

#include "Ray.h"

// This design is subject to change
#include "constructFunctions.h"
#include "surfaces\Plane.h"
#include "surfaces\Sphere.h"
#include "surfaces\OBB.h"

#include "json.hpp"

#include <SFML\Graphics\Image.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Sprite.hpp>

#include <vector>
#include <fstream>

Application::Application() :
	m_currentScene(0),
	m_sceneLoader("scenes")
{
	loadConfiguration();
	std::srand(static_cast<unsigned>(std::time(nullptr)));

	// Set up scene
	m_sceneLoader.pushConstructFunction("spheres", createSphere);
	m_sceneLoader.pushConstructFunction("planes", createPlane);
	m_sceneLoader.pushConstructFunction("obbs", createOBB);

	m_sceneLoader.pushConstructFunction("points", createLight);

	// Set up the image
	m_image.create(m_config.imageWidth, m_config.imageHeight);
	m_camera = new Camera(m_config.imageWidth, m_config.imageHeight, 90.f);

	// Load first scene
	m_scene = m_sceneLoader.load(m_currentScene);
	if (!m_scene)
	{
		std::printf("WARNING - No scene has been loaded");
	}
}

void Application::run()
{
	std::printf(
		"\n--------\n"
		"Commands:\n"
		" ] = increase fov angle\n"
		" [ = decrease fov angle\n"
		" \\ = fov angle to 90 deg\n"
		" P = Reset camera\n"
		" O = render BVH\n"
		" = = increase BVH render depth\n"
		" - = decrease BVH render depth\n"
		" 0-9 = load any of the scenes\n"
		" Esc = Exit\n"
		" F = save render to file\n");
	
	// Set up the window
	sf::RenderWindow window;
	window.create(sf::VideoMode(m_config.imageWidth,m_config.imageHeight), "Ray Tracer",sf::Style::Default);
	window.setFramerateLimit(m_config.fps);
	window.setSize(sf::Vector2u(m_config.windowWidth, m_config.windowHeight));
	window.setPosition(sf::Vector2i(
			sf::VideoMode::getDesktopMode().width / 2 - m_config.windowWidth / 2, 
			sf::VideoMode::getDesktopMode().height /2 - m_config.windowHeight /2));

	sf::Texture texture;
	sf::Sprite sprite;
	texture.loadFromImage(m_image);
	sprite.setTexture(texture);

	// Create variables used for time and loops
	sf::Clock elapsedTimeClock;
	float totalElapsedTime = 0.f;
	float totalFrameTime = 0.f;
	float fps = 0.f;
	int currentX = 0;
	int currentY = 0;

	//m_camera->transformCamToWorld(mat4::lookat(vec3(0,1,0), vec3(0, 0, -1), vec3(0, 1, 0)));
	sf::Event event;
	while (window.isOpen())
	{
		// This can happen when the used drags around the window
		if (elapsedTimeClock.getElapsedTime().asSeconds() > m_config.maxRenderTime*10.f)
			elapsedTimeClock.restart();

		const float elapsedTime = elapsedTimeClock.restart().asSeconds();
		totalFrameTime += elapsedTime;
		
		// Poll windows and move camera
		pollEvents(window, event);
		if (window.hasFocus())
		{
			transformCamera(elapsedTime);
		}

		// Update camera rotation position
		// Render image
		m_camera->updateCamToWorld();
		bool quitEarly = false;
		for (int &pX = currentX; !quitEarly && pX < m_config.imageWidth; pX++)
		{
			for (int &pY = currentY; !quitEarly && pY < m_config.imageHeight; pY++)
			{
				const vec3 color = m_rayTracer.calculatePixelColor(m_camera->createRay(pX, pY), *m_scene);
				m_image.setPixel(pX, pY, sf::Color(
					static_cast<sf::Uint8>(color[0]),
					static_cast<sf::Uint8>(color[1]),
					static_cast<sf::Uint8>(color[2])));
				if (elapsedTimeClock.getElapsedTime().asSeconds() > m_config.maxRenderTime)
					quitEarly = true;
			}
			if (quitEarly)
			{
				pX--;
				continue;
			}
			currentY = 0;
		}

		if (!quitEarly)
		{
			currentX = 0;
			currentY = 0;
			fps = 1.f / totalFrameTime;
			totalFrameTime = 0.f;
		}

		// Update title
		char title[128];
		vec3 pos = m_camera->getCamToWorld() * vec3();
		sprintf_s(title, "Ray Tracer Scene %i - BVH depth: %i - FPS: %.2f - totalTime: %.2f - X: %.2f Y: %.2f Z: %.2f\n",
			m_currentScene,
			m_rayTracer.m_BVHDepth,
			fps != fps ? 0.f : fps,
			totalElapsedTime += elapsedTime,
			pos[0],pos[1],pos[2]);

		window.setTitle(title);

		// Draw image
		window.clear();
		texture.update(m_image);
		window.draw(sprite);
		window.display();
	}

	delete m_camera;
}

bool Application::loadConfiguration()
{
	const char* configPath = "config.json";
	std::ifstream configFile(configPath);
	if (!configFile.is_open())
	{
		std::printf("ERROR: \'%s\' could not be loaded\n", configPath);
		return false;
	}

	nlohmann::json config;
	configFile >> config;

	m_config.fps			= config["fps"].get<int>();
	m_config.imageWidth		= config["imageWidth"].get<int>();
	m_config.imageHeight	= config["imageHeight"].get<int>();
	m_config.windowWidth	= config["windowWidth"].get<int>();
	m_config.windowHeight	= config["windowHeight"].get<int>();
	m_config.maxRenderTime	= config["maxRenderTime"].get<float>();

	std::printf(
		"Loaded configuration:\n"
		"fps: %i\n"
		"imageWidth: %i\n"
		"imageHeight: %i\n"
		"windowWidth: %i\n"
		"windowHeight: %i\n"
		"maxRenderTime: %f\n",
		m_config.fps,
		m_config.imageWidth,
		m_config.imageHeight,
		m_config.windowWidth,
		m_config.windowHeight, 
		m_config.maxRenderTime);

	configFile.close();
	return true;
}

void Application::pollEvents(sf::RenderWindow & window, sf::Event & event)
{
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
		{
			switch (event.key.code)
			{
				// Save render
			case sf::Keyboard::Key::F:
			{
				// Safe the image to a file
				char path[256];
				tm timeinfo;
				time_t rawtime;
				std::time(&rawtime);
				localtime_s(&timeinfo, &rawtime);

				sprintf_s(path, "images/%.2i%.2i%.2i_%.2i%.2i%.2i.png",
					timeinfo.tm_year + 1900,
					timeinfo.tm_mon + 1,
					timeinfo.tm_mday,
					timeinfo.tm_hour,
					timeinfo.tm_min,
					timeinfo.tm_sec);
				m_image.saveToFile(path);
				std::printf("Saved render to %s", path);
			}
			break;
			
			case sf::Keyboard::Key::O:
			{
				m_rayTracer.m_renderBVH = !m_rayTracer.m_renderBVH;
			}
			break;

			case sf::Keyboard::Key::Dash:
			{
				int bvhDepth = m_rayTracer.m_BVHDepth;
				bvhDepth = clamp(--bvhDepth, 0, bvhDepth);

				m_rayTracer.m_BVHDepth = static_cast<unsigned>(bvhDepth);
			}
			break;

			case sf::Keyboard::Key::Equal:
			{
				int bvhDepth = m_rayTracer.m_BVHDepth;
				bvhDepth = clamp(++bvhDepth, 0, bvhDepth);

				m_rayTracer.m_BVHDepth = static_cast<unsigned>(bvhDepth);
			}
			break;

			// Get correct scene
			case sf::Keyboard::Key::Num0:
			case sf::Keyboard::Key::Num1:
			case sf::Keyboard::Key::Num2:
			case sf::Keyboard::Key::Num3:
			case sf::Keyboard::Key::Num4:
			case sf::Keyboard::Key::Num5:
			case sf::Keyboard::Key::Num6:
			case sf::Keyboard::Key::Num7:
			case sf::Keyboard::Key::Num8:
			
				Scene *scene;
				scene = m_sceneLoader.load(event.key.code - sf::Keyboard::Key::Num0);
				if (scene)
				{
					m_currentScene = event.key.code - sf::Keyboard::Key::Num0;
					std::printf("Loaded scene %i\n", m_currentScene);

					delete m_scene;
					m_scene = scene;
				}
			
				break;

			case sf::Keyboard::Key::Num9:
			{
				std::vector<Primitive*> prims;
				std::vector<Light*> lights;
				lights.push_back(new Light(vec3(0, 30, -45), 0.8f, vec3(255, 255, 255)));
				prims.push_back(new Primitive(
					new Plane(vec3(0,-20,0),vec3(0,1,0),vec3(0,0,0),false), 
					Material(vec3(255, 255, 255), 100, 1.f, 1.f)));
				prims.push_back(new Primitive(
					new Sphere(vec3(0, -10, -45), 10.f),
					Material(vec3(230,230,230),10000.f,1.f,0.5f)));

				std::printf("Generating random scene\n");

				//for (int i = 0; i < 10; i++)
				//{
				//		prims.push_back(new Primitive(
				//		new OBB(
				//			vec3(i * 5.f, -20.f, 0.f), 
				//			vec3(4, 10, 4), vec3()),
				//		Material(vec3(100,(float)i * 20,100), 100.f, 1.f, 1.f)));
				//}

				//for (int i = 0; i < 3; i++)
				//{
				//	prims.push_back(new Primitive(
				//		new Sphere(
				//			vec3(i * 5.f, -2.f, -5.f),
				//			2.f),
				//		Material(vec3(100, (float)i * 255, 100), 100.f, 1.f, 1.f)));
				//}

				for (int i = 0; i < 1000; i++)
				{
					const vec3 color(
						static_cast<float>(std::rand() % 255), 
						static_cast<float>(std::rand() % 255), 
						static_cast<float>(std::rand() % 255));
					const vec3 rotation(
						static_cast<float>(std::rand() % 16 - 8), 
						static_cast<float>(std::rand() % 16 - 8), 
						static_cast<float>(std::rand() % 16 - 8));
					float x = 0.f + (std::rand() % 300);
					float z = 0.f;
					while(z > -10.f)
						z -=(std::rand() % 200);
					const float height= 5.f + (std::rand() % 100) / 100.f * 15.f;
					const float width = 5.f + (std::rand() % 100) / 100.f * 4.f;
					const float depth = 5.f + (std::rand() % 100) / 100.f * 4.f;

					switch (i%4)
					{
					case 0:
						x *= -1;
						break;
					case 2:
						x *= -1;
						z = 0.f + (std::rand() % 25);
						break;

					case 3:
						z = 0.f + (std::rand() % 25);
						break;
					}

					prims.push_back(new Primitive(
						new OBB(
							vec3(x, -20.f, z - 35.f), 
							vec3(width, height, depth), rotation),
						Material(color, 100.f, 1.f, 1.f)));
				}

				std::printf("Finished generating random scene\n");

				delete m_scene;
				m_scene = new Scene(prims, lights, 0.2f, vec3(180, 180, 180));
			}
			break;

				// Close window
			case sf::Keyboard::Key::Escape:
				window.close();
				break;
			}
		}
		break;

		case sf::Event::Closed:
			window.close();
			break;
		}
	}
}

void Application::transformCamera(const float elapsedTime)
{
	// Check camera related movement
	// Move
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		m_camera->translate(vec3(0, 0, -1.f*elapsedTime));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		m_camera->translate(vec3(-1.f*elapsedTime, 0, 0));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		m_camera->translate(vec3(0, 0, 1.f*elapsedTime));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		m_camera->translate(vec3(1.f*elapsedTime, 0, 0));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		m_camera->translate(vec3(0, 1.f*elapsedTime, 0));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
		m_camera->translate(vec3(0, -1.f*elapsedTime, 0));

	// Rotate
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_camera->rotateX(90 * elapsedTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_camera->rotateY(90 * elapsedTime);
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_camera->rotateX(-90 * elapsedTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_camera->rotateY(-90 * elapsedTime);

	// Change FOV
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RBracket))
		m_camera->addFovAngle(45.f * elapsedTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LBracket))
		m_camera->addFovAngle(-45.f * elapsedTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSlash))
		m_camera->setFovAngle(90);

	// Reset camera
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
		m_camera->resetCamera();
}