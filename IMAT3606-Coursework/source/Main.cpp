#include "RenderGL.h"
#include "Engine.h"

#include <cstdlib>
#include <stdio.h>


#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1024
RenderGL renderer = RenderGL(WINDOW_WIDTH, WINDOW_HEIGHT);
shared_ptr<Model> modelTest = std::make_shared<Model>(&renderer);
Engine engine = Engine();

int main() {
	engine.init();
	renderer.init();
	Input::getInstance().registerMouseListener(renderer.buttonTest);
	modelTest->init("./cube.obj", "./texture.bmp");
	renderer.addModel(modelTest); //todo move models into Screen class that manages what to draw

	engine.setModules(&renderer);
	engine.mainLoop();
	engine.exit();
	
}