#pragma once
#ifndef LEVELLOADER_H
#define LEVELLOADER_H
#include <Engine.h>
#include <Graphics.h>
#include "OnClickFunctions.h"
#include "tinyxml2.h"

class LevelLoader {
public:
	static void loadLevel(Engine* engine, Graphics* renderer, const char* filePath) 
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile(filePath);
		tinyxml2::XMLElement* screenElement = doc.FirstChildElement("screen");
		const char* type = screenElement->Attribute("type");
		if (strcmp(type, "menu") == 0) {
			loadMenu(engine, renderer, screenElement);
		}
	}
private:
	static void loadMenu(Engine* engine, Graphics* renderer, tinyxml2::XMLElement* screenElement)
	{
		shared_ptr<MenuScreen> menuScreen(new MenuScreen(renderer, engine));
		tinyxml2::XMLElement* stringElement = screenElement->FirstChildElement("strings")->FirstChildElement();
		while (stringElement != NULL) {
			loadStringElement(renderer, menuScreen, stringElement);
			stringElement = stringElement->NextSiblingElement();
		}
		tinyxml2::XMLElement* buttonElement = screenElement->FirstChildElement("buttons")->FirstChildElement();
		while (buttonElement != NULL) {
			loadButtonElement(engine, renderer, menuScreen, buttonElement);
			buttonElement = buttonElement->NextSiblingElement();
		}
		engine->registerScreen(menuScreen);
	}

	static void loadStringElement(Graphics* renderer, shared_ptr<MenuScreen> menuScreen, tinyxml2::XMLElement* stringElement)
	{
		Font font = *AssetManager::getInstance()->getFont("./resources/fonts/arial.ttf", renderer);
		const char* text = stringElement->FirstChildElement("value")->GetText();
		glm::vec3 pos = glm::vec3(stringElement->FirstChildElement("position")->FirstChildElement("x")->FloatText(), stringElement->FirstChildElement("position")->FirstChildElement("y")->FloatText(), -1.0);
		glm::vec3 scale = glm::vec3(stringElement->FirstChildElement("scale")->FirstChildElement("x")->FloatText(), stringElement->FirstChildElement("scale")->FirstChildElement("y")->FloatText(), stringElement->FirstChildElement("scale")->FirstChildElement("z")->FloatText());
		shared_ptr<TextBox> textBox = std::make_shared<TextBox>(text, font, Transform(pos, scale, glm::quat()), renderer);
		menuScreen->addTextBox(textBox);
	}

	static void loadButtonElement(Engine* engine, Graphics* renderer, shared_ptr<MenuScreen> menuScreen, tinyxml2::XMLElement* buttonElement) 
	{
		//TODO clean up duplicated code
		Font font = *AssetManager::getInstance()->getFont("./resources/fonts/arial.ttf", renderer);
		const char* text = buttonElement->FirstChildElement("value")->GetText();
		glm::vec3 pos = glm::vec3(buttonElement->FirstChildElement("position")->FirstChildElement("x")->FloatText(), buttonElement->FirstChildElement("position")->FirstChildElement("y")->FloatText(), -1.0);
		glm::vec3 scale = glm::vec3(buttonElement->FirstChildElement("scale")->FirstChildElement("x")->FloatText(), buttonElement->FirstChildElement("scale")->FirstChildElement("y")->FloatText(), buttonElement->FirstChildElement("scale")->FirstChildElement("z")->FloatText());
		Transform transform(pos, scale, glm::quat());
		shared_ptr<Button> button(new Button(text, font, transform, renderer));
		menuScreen->addButton(button);
		Input::getInstance().registerMouseListener(button);
		string funcName = string(buttonElement->FirstChildElement("function")->Attribute("type"));
		loadButtonFunc(funcName, buttonElement, button, engine);
	}

	static void loadButtonFunc(string funcName, tinyxml2::XMLElement* buttonElement, shared_ptr<Button> button, Engine* engine)
	{
		switch (OnClickFunctions::getFunctionType(funcName))
		{
			case OnClickFunctions::FunctionType::SWITCH_SCREEN: {
				tinyxml2::XMLElement* paramElement = buttonElement->FirstChildElement("function")->FirstChildElement("params")->FirstChildElement();
				while (paramElement != NULL) {
					string paramName = paramElement->Attribute("name");
					std::transform(paramName.begin(), paramName.end(), paramName.begin(), std::tolower);
					if (paramName == "screenid") break;
					paramElement = paramElement->NextSiblingElement();
				}
				button->addOnClickFn(std::bind(OnClickFunctions::switchScreen, engine, paramElement->IntText()));
				break;
			}
			case OnClickFunctions::FunctionType::EXIT:
			{
				button->addOnClickFn(std::bind(OnClickFunctions::exit, engine));
				break;
			}
			case OnClickFunctions::FunctionType::DO_NOTHING:
			default:
			{
				button->addOnClickFn(std::bind(OnClickFunctions::doNothing));
				break;
			}
		}
	}
};

#endif // !LEVELLOADER_H

