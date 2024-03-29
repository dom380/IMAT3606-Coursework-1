#include "Engine.h"
#include <Graphics\WindowGLFW.h>
#include <Renderers\RenderGL.h>
#include <algorithm>
#include <utils\tinyxml2.h>
#include <utils\OnClickFunctions.h>
#include <utils\LevelLoader.h>
#include <Screens\LoadingScreen.h>
#include <InputGLFW.h>


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
	inputHandler = buildInput(inputImplementation);
	window = buildWindow(graphicsContext);
	bool success = window->inititalise();
	if (!success)
	{
		std::cerr << "Failed to create window. Exiting..." << std::endl;
		exit();
	}
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
	while (!window->shouldExit()) {
		double newTime = timer.getElapsedTime();
		double frameTime = newTime - currentTime; 
		currentTime = newTime; //set current time

		while (frameTime > 0.0) //While there is still time to update the simulation
		{
			double deltaTime = std::min(frameTime, dt);
			activeScreen.second->update(deltaTime);

			frameTime -= deltaTime;
			t += deltaTime;
			//todo pass t and delta time to physics sim here
		}
		renderer->prepare();
		activeScreen.second->render();
		window->display();
		window->pollEvents();
	}
}

void Engine::exit()
{
	if (closed) return;
	timer.stop();
	if(renderer != nullptr)
		renderer->exit();
	AssetManager::getInstance()->exit();
	if(window != nullptr)
		window->close();
	closed = true;
	std::exit(0);
}

string Engine::registerScreen(shared_ptr<Screen> screen)
{
	gameScreens.emplace(std::pair<string, shared_ptr<Screen>>(screen->getID(), screen));
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
		activeScreen = std::pair<string, shared_ptr<Screen>>("LoadingScreen", std::make_shared<LoadingScreen>(window, this, renderer, inputHandler, screenId));
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
		activeScreen = std::pair<string, shared_ptr<Screen>>("LoadingScreen", std::make_shared<LoadingScreen>(window, this, renderer, inputHandler, screenId));
	} else{
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
		system("pause");
		exit();
	}
	tinyxml2::XMLElement* element = config.RootElement();
	width = element->FirstChildElement("width")!=NULL ? element->FirstChildElement("width")->IntText(1024) : 1024;
	height = element->FirstChildElement("height")!=NULL ? element->FirstChildElement("height")->IntText(720) : 720;
	initialScreenId = element->FirstChildElement("initScreen") != NULL ? element->FirstChildElement("initScreen")->GetText() : "MainMenu";
	string renderer = element->FirstChildElement("renderer")!= NULL ? element->FirstChildElement("renderer")->GetText() : "OPEN_GL";
	graphicsContext = enumParser.parse(renderer);

	string resourceLocation = element->FirstChildElement("fontLocation") != NULL ? element->FirstChildElement("fontLocation")->GetText() : "./resources/fonts/";
	AssetManager::getInstance()->setAssetFolder(resourceLocation, AssetManager::ResourceType::FONT);

	resourceLocation = element->FirstChildElement("levelLocation") != NULL ? element->FirstChildElement("levelLocation")->GetText() : "./resources/levels/";
	AssetManager::getInstance()->setAssetFolder(resourceLocation, AssetManager::ResourceType::LEVEL);

	resourceLocation = element->FirstChildElement("modelLocation") != NULL ? element->FirstChildElement("modelLocation")->GetText() : "./resources/models/";
	AssetManager::getInstance()->setAssetFolder(resourceLocation, AssetManager::ResourceType::MODEL);

	resourceLocation = element->FirstChildElement("textureLocation") != NULL ? element->FirstChildElement("textureLocation")->GetText() : "./resources/textures/";
	AssetManager::getInstance()->setAssetFolder(resourceLocation, AssetManager::ResourceType::TEXTURE);

	resourceLocation = element->FirstChildElement("shaderLocation") != NULL ? element->FirstChildElement("shaderLocation")->GetText() : "./shaders/";
	AssetManager::getInstance()->setAssetFolder(resourceLocation, AssetManager::ResourceType::SHADER);

	auto inputEnumParser = EnumParser<Input::InputImpl>();
	string input = element->FirstChildElement("input") != NULL ? element->FirstChildElement("input")->GetText() : "GLFW";
	inputImplementation = inputEnumParser.parse(input);
}

void Engine::loadFirstLevel()
{
	string levelPath = AssetManager::getInstance()->getRootFolder(AssetManager::ResourceType::LEVEL) + initialScreenId + ".xml";
	if (!LevelLoader::loadLevel(this, renderer, inputHandler, levelPath.c_str())) {
		std::cerr << "Failed to load initial level..." << std::endl << "Exiting" << std::endl;
		std::exit(1);
	}
	this->switchScreen(initialScreenId);
}

int Engine::getWindowWidth()
{
	return width;
}

int Engine::getWindowHeight()
{
	return height;
}

string Engine::getInitialScreenId()
{
	return initialScreenId;
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

shared_ptr<Window> Engine::buildWindow(GraphicsContext context)
{
	switch (context)
	{
	case GraphicsContext::OPEN_GL:
	{
		return std::make_shared<WindowGLFW>(width, height, std::dynamic_pointer_cast<InputGLFW, Input>(inputHandler)); //Assume using GLFW input for GLFW window...
	}
	default: //Default to GLFW
		return std::make_shared<WindowGLFW>(width, height, std::dynamic_pointer_cast<InputGLFW, Input>(inputHandler));
	};
}

shared_ptr<Input> Engine::buildInput(Input::InputImpl impl)
{
	switch (impl)
	{
	case Input::InputImpl::GLFW:
		return std::make_shared<InputGLFW>();
	case Input::InputImpl::GLUT:
		return nullptr; //TODO... maybe
	default: //Default to GLFW
		return std::make_shared<InputGLFW>();
	}
}
