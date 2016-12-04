#pragma once
#ifndef ENGINE_H
#define ENGINE_H
#include "AssetManager.h"
#include "Graphics.h"
#include "GL/glfw3.h"
class Engine {
public:

	Engine();
	~Engine();
	/*
		
	*/
	void init();
	void mainLoop();
	void exit();

	void setModules(Graphics* graphics);
private:
	Graphics* renderer;
	GLFWwindow* window;

	bool closed;
};

#endif // !ENGINE_H

