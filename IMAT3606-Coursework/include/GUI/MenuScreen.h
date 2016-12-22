#pragma once
#ifndef MENUSCREEN_H
#define MENUSCREEN_H
#include <Screen.h>
#include <GUI\Button.h>
#include <AssetManager.h>
#include <Engine.h>
#include <memory>
using std::shared_ptr;

class MenuScreen : public Screen {
public:
	MenuScreen() {};
	~MenuScreen() {};
	MenuScreen(Graphics* graphics, Engine* engine);
	void render(float dt);
	void resize(int width, int height);
	void dispose();
	//todo manage these properly
	shared_ptr<Button> buttonTest;
private:
	Graphics* graphics;
	Engine* engine;
	
};

#endif // !SCREENMENU_H

