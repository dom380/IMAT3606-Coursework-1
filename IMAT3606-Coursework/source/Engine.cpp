#include "Engine.h"
#include <algorithm>
#include <utils\tinyxml2.h>
#include <utils\OnClickFunctions.h>
#include <utils\LevelLoader.h>


Engine::Engine()
{
	closed = false;
	timer = Timer();
}

Engine::Engine(float w, float h)
{
	width = w;
	height = h;
	closed = false;
	timer = Timer();
}

Engine::~Engine()
{
	exit();
}

void Engine::init()
{
	// Initialize GLFW
	if (!glfwInit()) exit();

	//	 Select OpenGL 4.0 with a forward compatible core profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_DOUBLEBUFFER, true);

	// Open the window
	window = glfwCreateWindow(width, height, "IMAT3606-CourseWork", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit();
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, &inputHandler.keyboardCallback);
	glfwSetMouseButtonCallback(window, &inputHandler.mouseButtonCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
}

void Engine::mainLoop()
{
	double t = 0.0;
	double dt = 1 / 60.0;
	timer.start();
	auto currentTime = timer.getElapsedTime();
	//variable timestep
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		double newTime = timer.getElapsedTime();
		double frameTime = newTime - currentTime; 
		currentTime = newTime; //set current time

		while (frameTime > 0.0) //While there is still time to update the simulation
		{
			float deltaTime = std::min(frameTime, dt);
			renderer->prepare();
			activeScreen.second->update(dt);

			frameTime -= deltaTime;
			t += deltaTime;
		}
		activeScreen.second->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Engine::exit()
{
	if (closed) return;
	timer.stop();
	renderer->exit();
	AssetManager::getInstance()->exit();

	// Close window and terminate GLFW
	glfwTerminate();
	closed = true;
	std::exit(0);
}

void Engine::setModules(Graphics * graphics)
{
	renderer = graphics;
}

unsigned int Engine::registerScreen(shared_ptr<Screen> screen)
{
	currentScreenId++;
	gameScreens.emplace(std::pair<unsigned int, shared_ptr<Screen>>(currentScreenId, screen));
	screen->setID(currentScreenId);
	return currentScreenId;
}

shared_ptr<Screen> Engine::getActiveScreen()
{
	return activeScreen.second;
}

void Engine::switchScreen(unsigned int screenId)
{
	auto it = gameScreens.find(screenId);
	if (it == gameScreens.end()) {
		//todo throw exeception
	}
	else {
		activeScreen = *it;
	}
}

void Engine::replaceScreen(unsigned int screenId)
{
	auto it = gameScreens.find(screenId);
	if (it == gameScreens.end()) {
		//todo throw exeception
	} else{
		activeScreen.second.reset();
		unsigned int idToRemove = activeScreen.first;
		activeScreen = *it;
		gameScreens.erase(idToRemove);
	}
}

void Engine::loadConfig()
{
	tinyxml2::XMLDocument config;
	tinyxml2::XMLError ec = config.LoadFile("config.xml");
	//TODO load game screen from xml
	//TODO map screens to xml file name, if not there load in, avoids issues with ID and loading order?
	LevelLoader::loadLevel(this, renderer, "./resources/levels/MainMenu.xml");
	this->switchScreen(1);
}
