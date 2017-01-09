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
	TextBox(string text, Font textfont, Transform pos, shared_ptr<Graphics>& graphics, glm::vec3& colour = glm::vec3(1.0, 1.0, 1.0), string id = "");
	TextBox(const char* text, Font textfont, Transform pos, shared_ptr<Graphics>& graphics, glm::vec3& colour = glm::vec3(1.0, 1.0, 1.0), string id = "");
	~TextBox() {};
	TextBox& operator=(TextBox& other);
	void updateText(string newText);
	void render();
	string getId();
private:
	void init(Font textfont, Transform pos);
	string text;
	Font font;
	shared_ptr<Shader> textShader;
	Transform transform;
	unsigned int VAO, VBO;
	shared_ptr<Graphics> graphics;
	float charX, charY; //Current position of next character to render
	glm::vec3 textColour;
	bool haveVAO = false;
	string id;
};

#endif // !TEXTBOX_H

