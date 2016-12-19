#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include <string>
using std::string;
#include "EventListener.h"
#include "Font.h"
#include "Shader.h"
#include <utils\Transform.h>
#include <Graphics.h>
#include <gl\glm\glm\gtc\matrix_transform.hpp>
#include <algorithm> 

template <typename OnClick> //Maybe change this to use function pointer?
class Button : public EventListener {
private:
	struct AABB {
		float x, y, width, height;
	};
	AABB aabb;
	unsigned int VAO, VBO;
	Graphics* graphics;
	float charX, charY; //Current position of next character to render

	void init(Font textfont, Transform pos) {
		font = textfont;
		transform = pos;
		charX = pos.position.x;
		charY = pos.position.y;
		graphics->buildTextShader(VAO, VBO, textShader);
		buildAABB();
	};

	void buildAABB() {
		float maxHeight = 0;
		aabb.x = transform.position.x;
		aabb.y = transform.position.y;
		aabb.width = 0;
		for (char c : text) {
			Font::Character character = font.getChar(c);
			aabb.y = std::min((transform.position.y - (character.size.y - character.bearing.y) * transform.scale.y), aabb.y);
			maxHeight = std::max(character.size.y * transform.scale.y, maxHeight);
			aabb.width += (character.offset >> 6) *  transform.scale.x;
		}
		aabb.height = (transform.position.y - aabb.y) + maxHeight; //Height of AABB is the maximum character height add the maximum underline bearing
	};
	
protected:
	string text;
	Font font;
	Shader textShader;
	Transform transform;
	OnClick onClick;
public:
	//Constructors
	Button() {};
	Button(string text, Font textfont, Transform pos, Graphics* graphics) {
		this->text = text;
		this->graphics = graphics;
		init(textfont, pos);
	};
	Button(const char* text, Font textfont, Transform pos, Graphics* graphics) {
		this->text = string(text);
		this->graphics = graphics;
		init(textfont, pos);
	};
	~Button(){};
	Button& operator=(Button& other) {
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
	};

	/*
		Method to notify Button of Events it has subscribed to. 
		If the event was a click within the button's bounding box provided 
		OnClick method will be called.
	*/
	void handle(Event event) {
		//if not a click event we don't care
		if (event.eventType != EventType::CLICK) {
			return;
		}
		//if this button was clicked
		int x = event.x, y = graphics->getHeight() - event.y;
		if ((x >= aabb.x) && (x <= aabb.x + aabb.width) && (y >= aabb.y) && (y <= aabb.y + aabb.height))
		{
			onClick();
		}
	};
	
	//void addOnClickFn(OnClick c) {
	//	onClick = c;
	//};

	void render() {
		graphics->renderText(text, font, transform, VAO, VBO, textShader);
	}
};


#endif // !BUTTON_H
