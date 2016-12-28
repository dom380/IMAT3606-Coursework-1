#pragma once
#ifndef MENUSCREEN_H
#define MENUSCREEN_H
#include <Screen.h>
#include <GUI\Button.h>
#include <GUI\TextBox.h>
#include <AssetManager.h>
#include <Engine.h>
#include <memory>
using std::shared_ptr;
#include <vector>
using std::vector;
class Engine;
class MenuScreen : public Screen {
public:
	MenuScreen() {};
	~MenuScreen() {};
	MenuScreen(Graphics* graphics, Engine* engine);
	void update(double dt);
	void render();
	void resize(int width, int height);
	void dispose();
	void addButton(shared_ptr<Button> button);
	void addTextBox(shared_ptr<TextBox> textbox);

private:
	Graphics* graphics;
	Engine* engine;
	vector<shared_ptr<Button>> buttons;
	vector<shared_ptr<TextBox>> textBoxes;

};

#endif // !SCREENMENU_H

