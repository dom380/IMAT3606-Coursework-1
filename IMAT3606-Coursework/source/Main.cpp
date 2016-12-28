#include "RenderGL.h"
#include "Engine.h"
#include "GUI\MenuScreen.h"
#include "GameScreen.h"
#include <cstdlib>
#include <stdio.h>


#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1024
RenderGL renderer = RenderGL(WINDOW_WIDTH, WINDOW_HEIGHT);
Engine engine = Engine(WINDOW_WIDTH, WINDOW_HEIGHT);

int main() {
	
	engine.init();
	renderer.init();
	engine.setModules(&renderer);
	engine.loadConfig();
	//shared_ptr<Screen> mainMenu(new MenuScreen(&renderer, &engine));
	shared_ptr<Screen> gameScreen(new GameScreen(&renderer));
	
	//engine.registerScreen(mainMenu);
	//engine.switchScreen(engine.registerScreen(mainMenu));
	engine.registerScreen(gameScreen);
	//renderer.addModel(modelTest); //todo move models into Screen class that manages what to draw

	engine.mainLoop();
	engine.exit();
	return 0;
}