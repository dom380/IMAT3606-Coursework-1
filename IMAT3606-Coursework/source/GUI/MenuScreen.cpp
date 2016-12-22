#include <GUI\MenuScreen.h>
#include <GUI\OnClickTest.h>

MenuScreen::MenuScreen(Graphics* graphics, Engine* engine)
{
	this->engine = engine;
	this->graphics = graphics;
	string text("Start");
	shared_ptr<Font> font = AssetManager::getInstance()->getFont("./resources/fonts/arial.ttf", graphics);
	Transform transform(glm::vec3(30.0, 30.0, -1.0), glm::vec3(1.0, 1.0, 1.0), glm::quat());
	buttonTest = std::make_shared<Button>(text, *font, transform, graphics);
	Input::getInstance().registerMouseListener(buttonTest);
	//buttonTest->addOnClickFn(OnClickTest());
	auto fn = [](Engine* game) {
		//TODO think of a better way of handeling this screen change
		//Look into tinyXML 2 or cereal 
		game->switchScreen(2);
	};
	buttonTest->addOnClickFn(std::bind(fn, engine));
}

void MenuScreen::render(float dt)
{
	buttonTest->render();
}

void MenuScreen::resize(int width, int height)
{
}

void MenuScreen::dispose()
{
	buttonTest.reset();
}
