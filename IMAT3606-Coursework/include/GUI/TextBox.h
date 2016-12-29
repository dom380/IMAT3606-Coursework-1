#pragma once
#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "Graphics.h"
#include "Font.h"
#include <Graphics\Transform.h>
#include <string>
using std::string;

class TextBox {
public:
	//Constructors
	TextBox() {};
	TextBox(string text, Font textfont, Transform pos, shared_ptr<Graphics>& graphics);
	TextBox(const char* text, Font textfont, Transform pos, shared_ptr<Graphics>& graphics);
	~TextBox() {};
	TextBox& operator=(TextBox& other);

	void render();
private:
	void init(Font textfont, Transform pos);
	string text;
	Font font;
	Shader textShader;
	Transform transform;
	unsigned int VAO, VBO;
	shared_ptr<Graphics> graphics;
	float charX, charY; //Current position of next character to render
};

#endif // !TEXTBOX_H

