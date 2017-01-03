#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#define NOMINMAX
#include <string>
using std::string;
#include "EventListener.h"
#include "Font.h"
#include "Shader.h"
#include <Graphics\Transform.h>
#include <GUI\TextBox.h>
#include <Graphics.h>
#include <gl\glm\glm\gtc\matrix_transform.hpp>
#include <algorithm> 
#include <functional>
#include <memory>
using std::shared_ptr;


class Button : public EventListener {
private:
	struct AABB {
		float x, y, width, height;
	};
	AABB aabb;
	shared_ptr<Graphics> graphics;
	shared_ptr<TextBox> textbox;

	std::function<void()> onClickCallback;

	void init(Font textfont, Transform pos) {
		textbox = std::make_shared<TextBox>(text, textfont, pos, graphics);
		font = textfont;
		transform = pos;
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
	Transform transform;
public:
	//Constructors
	Button() {};
	Button(string text, Font textfont, Transform pos, shared_ptr<Graphics>& graphics) {
		this->text = text;
		this->graphics = graphics;
		init(textfont, pos);
	};
	Button(const char* text, Font textfont, Transform pos, shared_ptr<Graphics>& graphics) {
		this->text = string(text);
		this->graphics = graphics;
		init(textfont, pos);
	};
	~Button(){};
	Button& operator=(Button& other) {
		this->text = other.text;
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
	void handle(MouseEvent event) {
		//if not a click event we don't care
		if (event.eventType != MouseEventType::LEFT_CLICK) {
			return;
		}
		//if this button was clicked
		int x = event.x, y = graphics->getHeight() - event.y;
		if ((x >= aabb.x) && (x <= aabb.x + aabb.width) && (y >= aabb.y) && (y <= aabb.y + aabb.height))
		{
			onClickCallback();
		}
	};
	
	void handle(KeyEvent event) {
		
	}

	void addOnClickFn(std::function<void()> c) {
		onClickCallback = c;
	};

	void render() {
		textbox->render();
	}
};


#endif // !BUTTON_H
