#pragma once
#ifndef SCREEN_H
#define SCREEN
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
#include <GUI\TextBox.h>
/*
	Abstract base class that represents a single arbitrary 'Screen' within the game.
	Implementations of this class provide the best place for game specific code.
*/
class Screen {
public:
	/*
		Pure Virtual method. Implementations can use this to update an time based operations.
		This is called at least once (If current active screen) every frame before rendering.
		double dt, The time passed since last update call.
	*/
	virtual void update(double dt) = 0;
	/*
		Pure Virtual method. Implementations should call any rendering code here.
		This is called once (If current active screen) every frame.
	*/
	virtual void render() = 0;
	/*
		Pure Virtual method. Resize the screen.
		int width, New screen width.
		int height, New screen height.
	*/
	virtual void resize(int width, int height) = 0;
	/*
		Pure Virtual method. Implementations should free any resources here.
	*/
	virtual void dispose() = 0;
	virtual void addTextBox(shared_ptr<TextBox> textbox) = 0;
	/*
		Sets the screen's Id.
		string id, The Id.
	*/
	void setID(string id) {
		screenId = id;
	};
	/*
		Returns the screen's Id.
	*/
	string getID() {
		return screenId;
	}
protected:
	string screenId;
};

#endif // !SCREEN_H

