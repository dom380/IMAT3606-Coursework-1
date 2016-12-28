#pragma once
#ifndef GAMESCREEN_H
#define GAMESCREEN_H
#include <Model.h>
#include <Screen.h>
#include <memory>
using std::shared_ptr;
#include <vector>
using std::vector;

class GameScreen : public Screen
{
public:
	GameScreen(Graphics* renderer);
	~GameScreen() {};
	void update(double dt);
	void render();
	void resize(int width, int height);
	void dispose();

private:
	vector<shared_ptr<Model>> models;
	Graphics* renderer;

	double angle;
};

#endif // !GAMESCREEN_H

