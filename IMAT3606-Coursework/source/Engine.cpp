#include "Engine.h"
#include <RenderGL.h>
#include <algorithm>
#include <utils\tinyxml2.h>
#include <utils\OnClickFunctions.h>
#include <utils\LevelLoader.h>
#include <LoadingScreen.h>


Engine::Engine()
{
	closed = false;
	timer = Timer();
}

Engine::Engine(int width, int height)
{
	this->width = width;
	this->height = height;
	closed = false;
	timer = Timer();
}

Engine::~Engine()
{
	exit();
}

void Engine::init()
{
	//Read configuration file
	loadConfig();

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
	glfwSetCursorPosCallback(window, &inputHandler.mouseMovementCallback);

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, false);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	offscreen_context = glfwCreateWindow(640, 480, "", NULL, window);

	renderer = buildRenderer(graphicsContext);
	renderer->init();
	loadFirstLevel();
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
			double deltaTime = std::min(frameTime, dt);
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

string Engine::registerScreen(shared_ptr<Screen> screen)
{
	//currentScreenId++;
	gameScreens.emplace(std::pair<string, shared_ptr<Screen>>(screen->getID(), screen));
	//screen->setID(currentScreenId);
	return screen->getID();
}

shared_ptr<Screen> Engine::getActiveScreen()
{
	return activeScreen.second;
}

void Engine::switchScreen(string screenId)
{
	auto it = gameScreens.find(screenId);
	if (it == gameScreens.end()) {
		//try to load level
		//string path = "./resources/levels/" + screenId + ".xml";
		activeScreen = std::pair<string, shared_ptr<Screen>>("LoadingScreen", std::make_shared<LoadingScreen>(offscreen_context, this, renderer, screenId));
		/*if (LevelLoader::loadLevel(this, renderer, path.c_str()))
		{
			switchScreen(screenId);
		}
		else
		{
			std::cerr << "Failed to load level: " << screenId << std::endl;
		}*/
	}
	else {
		activeScreen = *it;
	}
}

void Engine::replaceScreen(string screenId)
{
	auto it = gameScreens.find(screenId);
	if (it == gameScreens.end()) {
		//try to load level
		//string path = "./resources/levels/" + screenId + ".xml";
		activeScreen = std::pair<string, shared_ptr<Screen>>("LoadingScreen", std::make_shared<LoadingScreen>(offscreen_context, this, renderer, screenId));
		//if (LevelLoader::loadLevel(this, renderer, path.c_str()))
		//{
		//	replaceScreen(screenId);
		//}
		//else
		//{
		//	std::cerr << "Failed to load level: " << screenId << std::endl;
		//}
	} else{
//		glfwMakeContextCurrent(window);
		activeScreen.second.reset();
		string idToRemove = activeScreen.first;
		activeScreen = *it;
		gameScreens.erase(idToRemove);
	}
}

void Engine::loadConfig()
{
	tinyxml2::XMLDocument config;
	tinyxml2::XMLError ec = config.LoadFile("config.xml");
	if (ec != tinyxml2::XMLError::XML_SUCCESS) {
		std::cerr << "Failed to read configuration. Exiting..." << std::endl;
		exit();
	}
	tinyxml2::XMLElement* element = config.RootElement();
	width = element->FirstChildElement("width")!=NULL ? element->FirstChildElement("width")->IntText(1024) : 1024;
	height = element->FirstChildElement("height")!=NULL ? element->FirstChildElement("height")->IntText(720) : 720;
	initialScreenId = element->FirstChildElement("initScreen") != NULL ? element->FirstChildElement("initScreen")->GetText() : "MainMenu";
	string renderer = element->FirstChildElement("renderer")!= NULL ? element->FirstChildElement("renderer")->GetText() : "OPEN_GL";
	graphicsContext = enumParser.parse(renderer);// GraphicsContext::OPEN_GL; 
}

void Engine::loadFirstLevel()
{
	string levelPath = "./resources/levels/" + initialScreenId + ".xml";
	if (!LevelLoader::loadLevel(this, renderer, levelPath.c_str())) {
		std::cerr << "Failed to load initial level..." << std::endl << "Exiting" << std::endl;
		std::exit(1);
	}
	this->switchScreen(initialScreenId);
}

shared_ptr<Graphics> Engine::buildRenderer(GraphicsContext renderType)
{
	switch (renderType) 
	{
	case GraphicsContext::OPEN_GL:
	{
		return std::make_shared<RenderGL>(width, height);
	}
	default: //Default to OpenGL
		return std::make_shared<RenderGL>(width, height);
	};
}
