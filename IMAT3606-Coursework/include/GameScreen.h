#pragma once
#ifndef GAMESCREEN_H
#define GAMESCREEN_H
#include <Model.h>
#include <Screen.h>
#include <memory>
#include <Graphics\Camera.h>
#include <Graphics\PerspectiveCamera.h>
using std::shared_ptr;
#include <vector>
using std::vector;
#ifndef NDEBUG
#include <utils\Timer.h>
#endif

class GameScreen : public Screen
{
public:
	GameScreen(shared_ptr<Graphics>& renderer, shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>());
	~GameScreen() {};
	void update(double dt);
	void render();
	void resize(int width, int height);
	void dispose();
	void addModel(shared_ptr<Model> model);
	void addLight(Light light);
	void updateLighting();
private:
	vector<shared_ptr<Model>> models;
	shared_ptr<Graphics> renderer;
	shared_ptr<Camera> camera;
	vector<Light> lights;
	double angle;
	unsigned int lightingBlockId;
	unsigned int lightingBufferId;
#ifndef NDEBUG
	Timer timer;
	string renderTime;
#endif
};

#endif // !GAMESCREEN_H

