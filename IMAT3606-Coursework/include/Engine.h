#pragma once
#ifndef ENGINE_H
#define ENGINE_H
#include "AssetManager.h"
#include "Graphics.h"
#include "GL/glfw3.h"
#include "Input.h"
#include "Screen.h"
#include <map>
using std::map;

class Engine {
public:

	Engine();
	~Engine();
	void init();
	void mainLoop();
	void exit();

	void setModules(Graphics* graphics);
	/*
		Registers the provided implementation of Screen with the engine. 
		Returns the ID of the newly registered screen. This ID is used in
		all subsequent calls. 
	*/
	unsigned int registerScreen(shared_ptr<Screen> screen);
	/*
		Returns a pointer to the current active screen.
	*/
	shared_ptr<Screen> getActiveScreen();
	/*
		Switches the current active screen to the screen with the specified ID.
		The screen must have been registered with the engine before hand.
		The previous screen will not be disposed of and will remain in memory.
	*/
	void switchScreen(unsigned int screenId);
	/*
		Replaces the current active screen to the screen with the specified ID.
		The screen must have been registered with the engine before hand.
		The previous screen will be disposed of and all none shared assets freed.
	*/
	void replaceScreen(unsigned int screenId);

private:
	Graphics* renderer;
	GLFWwindow* window;
	Input &inputHandler = Input::getInstance();
	map<unsigned int, shared_ptr<Screen>> gameScreens;
	std::pair<int,shared_ptr<Screen>> activeScreen;
	bool closed;
	unsigned int currentScreenId = 0;
};

#endif // !ENGINE_H

