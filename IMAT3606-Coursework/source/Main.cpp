#include "RenderGL.h"
#include "Engine.h"
#include "GUI\MenuScreen.h"
#include "GameScreen.h"
#include <cstdlib>
#include <stdio.h>

Engine engine = Engine();

int main() {
	engine.init();
	engine.mainLoop();
	engine.exit();
	return 0;
}