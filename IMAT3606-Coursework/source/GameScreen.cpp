#include "GameScreen.h"
#include <Input.h>

GameScreen::GameScreen(shared_ptr<Graphics>& renderer, shared_ptr<Camera> camera)
{
	this->renderer = renderer;
	this->camera = camera;
	this->camera->move(0.0, 15.0f, 0.0f);
	Input::getInstance().registerKeyListener(this->camera);
	Input::getInstance().registerMouseListener(this->camera);
}

void GameScreen::update(double dt)
{
#ifndef NDEBUG
	timer.start();
#endif
	//angle += dt;
	//if (angle>360.f)
	//{
	//	angle -= 360.f;
	//}

	//for (int i = 0; i < models.size() - 2; ++i)
	//{
	//	shared_ptr<Model> model = models.at(i);
	//	model->transform.orientation.x = 1.0f;
	//	model->transform.orientation.y = 1.0f;
	//	model->transform.orientation.z = 1.0f;
	//	model->transform.orientation.w = glm::radians(angle);
	//}
}

void GameScreen::render()
{
	for (shared_ptr<Model> model : models) {
		model->render(camera, lightingBufferId, lightingBlockId);
	}
#ifndef NDEBUG
	double elapsedTime = timer.getElapsedTimeMilliSec();
	timer.stop();
#endif
}

void GameScreen::resize(int width, int height)
{
}

void GameScreen::dispose()
{
	models.clear();
	lights.clear();
}

void GameScreen::addModel(shared_ptr<Model> model)
{
	models.push_back(model);
}

void GameScreen::addLight(Light light)
{
	lights.push_back(light);
}

void GameScreen::updateLighting()
{
	std::shared_ptr<Shader>shader = 
		AssetManager::getInstance()->getShader(std::pair<string, string>("./shaders/phong.vert", "./shaders/phong.frag"));
	renderer->bufferLightingData(lights, shader, lightingBufferId, lightingBlockId);
}
