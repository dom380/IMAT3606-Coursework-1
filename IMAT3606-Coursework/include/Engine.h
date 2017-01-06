#pragma once
#ifndef ENGINE_H
#define ENGINE_H
#define NOMINMAX
#include "utils\Timer.h"
#include "AssetManager.h"
#include "Graphics.h"
#include "GL/glfw3.h"
#include "Input.h"
#include "Screen.h"
#include "GUI\MenuScreen.h"
#include <utils\EnumParser.h>
#include <map>
using std::map;

class Engine {
public:

	Engine();
	Engine(int width, int height);
	~Engine();
	void init();
	void mainLoop();
	void exit();

	/*
		Registers the provided implementation of Screen with the engine. 
		Returns the ID of the newly registered screen. This ID is used in
		all subsequent calls. 
	*/
	string registerScreen(shared_ptr<Screen> screen);
	/*
		Returns a pointer to the current active screen.
	*/
	shared_ptr<Screen> getActiveScreen();
	/*
		Switches the current active screen to the screen with the specified ID.
		The screen must have been registered with the engine before hand.
		The previous screen will not be disposed of and will remain in memory.
	*/
	void switchScreen(string screenId);
	/*
		Replaces the current active screen to the screen with the specified ID.
		The screen must have been registered with the engine before hand.
		The previous screen will be disposed of and all none shared assets freed.
	*/
	void replaceScreen(string screenId);

	void loadConfig();

	void loadFirstLevel();

	enum GraphicsContext {
		OPEN_GL
	};

private:
	//Private members
	shared_ptr<Graphics> renderer;
	GLFWwindow* window;
	GLFWwindow* offscreen_context;
	Input &inputHandler = Input::getInstance();
	map<string, shared_ptr<Screen>> gameScreens;
	std::pair<string,shared_ptr<Screen>> activeScreen;
	bool closed;
	int width;
	int height;
	Timer timer;
	string initialScreenId;
	EnumParser<GraphicsContext> enumParser = EnumParser<GraphicsContext>();
	GraphicsContext graphicsContext;
	//Private Methods
	shared_ptr<Graphics> buildRenderer(GraphicsContext renderType);
};

#endif // !ENGINE_H

