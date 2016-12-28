#include "GameScreen.h"

GameScreen::GameScreen(Graphics * renderer)
{
	this->renderer = renderer;
	//todo read in from xml
	shared_ptr<Model> modelTest = std::make_shared<Model>(renderer);
	modelTest->init("./cube.obj", "./texture.bmp");
	models.push_back(modelTest);
}

void GameScreen::update(double dt)
{
	angle += dt;
	if (angle>360.f)
	{
		angle -= 360.f;
	}
	for (shared_ptr<Model> model : models) {
		model->transform.orientation.x = 1.0f;
		model->transform.orientation.y = 1.0f;
		model->transform.orientation.z = 1.0f;
		model->transform.orientation.w = glm::radians(angle);
	}
}

void GameScreen::render()
{
	for (shared_ptr<Model> model : models) {
		model->render();
	}
}

void GameScreen::resize(int width, int height)
{
}

void GameScreen::dispose()
{
}
