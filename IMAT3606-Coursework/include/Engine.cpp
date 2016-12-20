#include "Engine.h"
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1024
Engine::Engine()
{
	closed = false;
}

Engine::~Engine()
{
	exit();
}

void Engine::init()
{
	// Initialize GLFW
	if (!glfwInit()) exit();

	//	 Select OpenGL 4.3 with a forward compatible core profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_DOUBLEBUFFER, true);

	// Open the window
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "IMAT3606-CourseWork", NULL, NULL);
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
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		//todo change to use screens and add timer
		renderer->render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Engine::exit()
{
	if (closed) return;
	renderer->exit();
	AssetManager::getInstance()->exit();

	// Close window and terminate GLFW
	glfwTerminate();
	closed = true;

}

void Engine::setModules(Graphics * graphics)
{
	renderer = graphics;
}

unsigned int Engine::registerScreen(shared_ptr<Screen> screen)
{
	gameScreens.emplace(std::pair<unsigned int, shared_ptr<Screen>>(currentScreenId++, screen));
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
