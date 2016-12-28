#include "..\..\include\GUI\TextBox.h"

TextBox::TextBox(string text, Font textfont, Transform pos, Graphics * graphics)
{
	this->text = text;
	this->graphics = graphics;
	init(textfont, pos);
}

TextBox::TextBox(const char * text, Font textfont, Transform pos, Graphics * graphics)
{
	this->text = string(text);
	this->graphics = graphics;
	init(textfont, pos);
}

TextBox & TextBox::operator=(TextBox & other)
{
	this->charX = other.charX;
	this->charY = other.charY;
	this->text = other.text;
	this->VAO = other.VAO;
	this->VBO = other.VBO;
	this->textShader = other.textShader;
	this->transform = other.transform;
	this->font = other.font;
	this->graphics = other.graphics;
	return *this;
}

void TextBox::render()
{
	graphics->renderText(text, font, transform, VAO, VBO, textShader);
}

void TextBox::init(Font textfont, Transform pos)
{
	font = textfont;
	transform = pos;
	charX = pos.position.x;
	charY = pos.position.y;
	graphics->buildTextShader(VAO, VBO, textShader);
}
