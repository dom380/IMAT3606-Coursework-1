#include "RenderGL.h"
#include "Engine.h"
#include "GUI\MenuScreen.h"
#include "GameScreen.h"
#include <cstdlib>
#include <stdio.h>


#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1024
RenderGL renderer = RenderGL(WINDOW_WIDTH, WINDOW_HEIGHT);
Engine engine = Engine();

int main() {
	engine.init();
	renderer.init();
	shared_ptr<Screen> mainMenu(new MenuScreen(&renderer, &engine));
	shared_ptr<Screen> gameScreen(new GameScreen(&renderer));
	//Input::getInstance().registerMouseListener(mainMenu.buttonTest);
	//engine.registerScreen(mainMenu);
	engine.switchScreen(engine.registerScreen(mainMenu));
	engine.registerScreen(gameScreen);
	//renderer.addModel(modelTest); //todo move models into Screen class that manages what to draw

	engine.setModules(&renderer);
	engine.mainLoop();
	engine.exit();
	
}