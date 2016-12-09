#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include <string>
using std::string;
#include "EventListener.h"
#include "Font.h"

class Button : EventListener {
protected:
	string text;
	Font font;
public:
	Button(string text, Font textfont);
	~Button(){};
	void  handle(Event event);
	void render();
};


#endif // !BUTTON_H
