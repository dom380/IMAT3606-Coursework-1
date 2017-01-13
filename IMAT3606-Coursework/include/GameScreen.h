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
#include <Robot.h>
#include <GUI\TextBox.h>
#ifndef NDEBUG
#include <utils\Timer.h>
#endif

class GameScreen : public Screen, public EventListener
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
	void addTextBox(shared_ptr<TextBox> textbox);
	void updateLighting();

	void  handle(MouseEvent event);
	void  handle(KeyEvent event);
private:
	void updateScoreText();

	vector<shared_ptr<TextBox>> textBoxes;
	vector<shared_ptr<Model>> models;
	shared_ptr<Graphics> renderer;
	int activeCamera = 0;
	vector<shared_ptr<Camera>> cameras;
	vector<Light> lights;
	double angle = 0;
	double rotationSpeed = 50.0;
	unsigned int lightingBlockId = -1;
	unsigned int lightingBufferId = -1;
	shared_ptr<Robot> robot;
	int currentScore = 0;
#ifndef NDEBUG
	Timer timer;
	shared_ptr<TextBox> frameTime;
#endif
};

#endif // !GAMESCREEN_H

