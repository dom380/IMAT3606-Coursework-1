#pragma once
#ifndef SCREEN_H
#define SCREEN
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
#include <GUI\TextBox.h>

class Screen {
public:
	virtual void update(double dt) = 0;
	virtual void render() = 0;
	virtual void resize(int width, int height) = 0;
	virtual void dispose() = 0;
	virtual void addTextBox(shared_ptr<TextBox> textbox) = 0;
	void setID(string id) {
		screenId = id;
	};
	string getID() {
		return screenId;
	}
protected:
	string screenId;
};

#endif // !SCREEN_H

