#pragma once
#ifndef LEVELLOADER_H
#define LEVELLOADER_H
#include <Engine.h>
#include <Graphics.h>
#include <GUI\MenuScreen.h>
#include <GameScreen.h>
#include "OnClickFunctions.h"
#include "tinyxml2.h"

class LevelLoader {
public:
	static bool loadLevel(Engine* engine, shared_ptr<Graphics> renderer, const char* filePath) 
	{
		try
		{
			tinyxml2::XMLDocument doc;
			doc.LoadFile(filePath);
			tinyxml2::XMLElement* screenElement = doc.FirstChildElement("screen");
			const char* type = screenElement->Attribute("type");
			if (strcmp(type, "menu") == 0) {
				return loadMenu(engine, renderer, screenElement);
			}
			else if (strcmp(type, "level") == 0) {
				return loadGameLevel(engine, renderer, screenElement);
			}
			else
			{
				std::cerr << "Unrecognised screen type: " << type << std::endl;
				return false;
			}
		}
		catch (const std::runtime_error& re)
		{
			std::cerr << "Runtime error: " << re.what() << std::endl;
			return false;
		}
		catch (const std::exception& ex)
		{
			std::cerr << "Error occurred: " << ex.what() << std::endl;
			return false;
		}
		catch (...)
		{
			std::cerr << "Unknown failure occurred. Possible memory corruption" << std::endl;
			return false;
		}
	}
private:
	static bool loadMenu(Engine* engine, shared_ptr<Graphics>& renderer, tinyxml2::XMLElement* screenElement)
	{
		shared_ptr<MenuScreen> menuScreen = std::make_shared<MenuScreen>(renderer, engine);
		menuScreen->setID(screenElement->Attribute("name"));
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
		return true;
	}

	static void loadStringElement(shared_ptr<Graphics>& renderer, shared_ptr<MenuScreen>& menuScreen, tinyxml2::XMLElement* stringElement)
	{
		Font font = *AssetManager::getInstance()->getFont("./resources/fonts/arial.ttf", renderer);
		const char* text = stringElement->FirstChildElement("value")!=NULL ? stringElement->FirstChildElement("value")->GetText() : "MISSING_STRING";
		Transform transform;
		loadTransform(transform, stringElement);
		shared_ptr<TextBox> textBox = std::make_shared<TextBox>(text, font, transform, renderer);
		menuScreen->addTextBox(textBox);
	}

	static void loadButtonElement(Engine* engine, shared_ptr<Graphics>& renderer, shared_ptr<MenuScreen> menuScreen, tinyxml2::XMLElement* buttonElement)
	{
		Font font = *AssetManager::getInstance()->getFont("./resources/fonts/arial.ttf", renderer);
		const char* text = buttonElement->FirstChildElement("value")!=NULL ? buttonElement->FirstChildElement("value")->GetText() : "MISSING_STRING";
		Transform transform;
		loadTransform(transform, buttonElement);
		shared_ptr<Button> button = std::make_shared<Button>(text, font, transform, renderer);
		menuScreen->addButton(button);
		Input::getInstance().registerMouseListener(button);
		string funcName = string(buttonElement->FirstChildElement("function")->Attribute("type"));
		loadButtonFunc(funcName, buttonElement, button, engine);
	}

	static void loadButtonFunc(string funcName, tinyxml2::XMLElement* buttonElement, shared_ptr<Button> button, Engine* engine)
	{
		switch (enumParser.parse(funcName))
		{
			case OnClickFunctions::FunctionType::SWITCH_SCREEN: {
				tinyxml2::XMLElement* paramElement = buttonElement->FirstChildElement("function")->FirstChildElement("params")->FirstChildElement();
				while (paramElement != NULL) {
					string paramName = paramElement->Attribute("name");
					std::transform(paramName.begin(), paramName.end(), paramName.begin(), std::tolower);
					if (paramName == "screenid") break;
					paramElement = paramElement->NextSiblingElement();
				}
				string temp = paramElement->GetText();
				button->addOnClickFn(std::bind(OnClickFunctions::switchScreen, engine, temp));
				break;
			}
			case OnClickFunctions::FunctionType::REPLACE_SCREEN:
			{
				tinyxml2::XMLElement* paramElement = buttonElement->FirstChildElement("function")->FirstChildElement("params")->FirstChildElement();
				while (paramElement != NULL) {
					string paramName = paramElement->Attribute("name");
					std::transform(paramName.begin(), paramName.end(), paramName.begin(), std::tolower);
					if (paramName == "screenid") break;
					paramElement = paramElement->NextSiblingElement();
				}
				string temp = paramElement->GetText();
				button->addOnClickFn(std::bind(OnClickFunctions::replaceScreen, engine, temp));
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

	static bool loadGameLevel(Engine* engine, shared_ptr<Graphics>& renderer, tinyxml2::XMLElement* screenElement) 
	{
		shared_ptr<GameScreen> gameScreen = std::make_shared<GameScreen>(renderer);
		gameScreen->setID(screenElement->Attribute("name"));
		tinyxml2::XMLElement* modelElement = screenElement->FirstChildElement("models")->FirstChildElement();
		while (modelElement != NULL) {
			loadModel(renderer, gameScreen, modelElement);
			modelElement = modelElement->NextSiblingElement();
		}
		tinyxml2::XMLElement* lightElement = screenElement->FirstChildElement("lights")->FirstChildElement();
		int lightCount = 0;
		while (lightElement != NULL && lightCount <10) //TODO define global light max
		{
			loadLight(gameScreen, lightElement);
			lightElement = lightElement->NextSiblingElement();
			lightCount++;
		}
		gameScreen->updateLighting();
		engine->registerScreen(gameScreen);
		return true;
	}

	static void loadModel(shared_ptr<Graphics>& renderer, shared_ptr<GameScreen> gameScreen, tinyxml2::XMLElement* modelElement)
	{
		shared_ptr<Model> model = std::make_shared<Model>(renderer);
		const char* modelPath = modelElement->FirstChildElement("file")->GetText();
		const char* texturePath = modelElement->FirstChildElement("texture")->GetText();
		model->init(modelPath, texturePath);
		loadTransform(model->transform, modelElement);
		gameScreen->addModel(model);
	}

	static void loadTransform(Transform &transform, tinyxml2::XMLElement* element) 
	{
		glm::vec3 pos;
		glm::vec3 scale;
		glm::quat quat; quat.y = 1.0f; quat.w = 0.0f;
		tinyxml2::XMLElement* posElement = element->FirstChildElement("position");
		if (posElement != NULL) {
			pos = glm::vec3(posElement->FirstChildElement("x")!=NULL ? posElement->FirstChildElement("x")->FloatText():0.0f, posElement->FirstChildElement("y")!=NULL ? posElement->FirstChildElement("y")->FloatText():0.0f, posElement->FirstChildElement("z")!=NULL ? posElement->FirstChildElement("z")->FloatText() : 0.0f);
		}
		tinyxml2::XMLElement* scaleElement = element->FirstChildElement("scale");
		if (scaleElement != NULL) {
			scale = glm::vec3(scaleElement->FirstChildElement("x")!=NULL ? scaleElement->FirstChildElement("x")->FloatText(1.0f):1.0f, scaleElement->FirstChildElement("y") != NULL ? scaleElement->FirstChildElement("y")->FloatText(1.0f):1.0f, scaleElement->FirstChildElement("z") != NULL ? scaleElement->FirstChildElement("z")->FloatText(1.0f) : 1.0f);
		}
		tinyxml2::XMLElement* quatElement = element->FirstChildElement("orientation");
		if (quatElement != NULL) {
			quat = glm::quat(quatElement->FirstChildElement("x")!=NULL ? quatElement->FirstChildElement("x")->FloatText():0.0f, quatElement->FirstChildElement("y")!=NULL ? quatElement->FirstChildElement("y")->FloatText(1.0f):1.0f, quatElement->FirstChildElement("z")!=NULL ? quatElement->FirstChildElement("z")->FloatText():0.0f, quatElement->FirstChildElement("w")!=NULL ? quatElement->FirstChildElement("w")->FloatText() : 0.0f);
		}
		transform.orientation = quat;
		transform.position = pos;
		transform.scale = scale;
	}

	static void loadLight(shared_ptr<GameScreen> gameScreen, tinyxml2::XMLElement* element)
	{
		Light light = Light();
		tinyxml2::XMLElement* posElement = element->FirstChildElement("position");
		if (posElement != NULL) 
		{
			light.pos = glm::vec3(posElement->FirstChildElement("x") != NULL ? posElement->FirstChildElement("x")->FloatText() : 0.0f, posElement->FirstChildElement("y") != NULL ? posElement->FirstChildElement("y")->FloatText() : 0.0f, posElement->FirstChildElement("z") != NULL ? posElement->FirstChildElement("z")->FloatText() : 0.0f);
		}
		tinyxml2::XMLElement* lightElement = element->FirstChildElement("ambient");
		if (lightElement != NULL)
		{
			light.ambient = glm::vec3(lightElement->FirstChildElement("r") != NULL ? lightElement->FirstChildElement("r")->FloatText() : 0.0f, lightElement->FirstChildElement("g") != NULL ? lightElement->FirstChildElement("g")->FloatText() : 0.0f, lightElement->FirstChildElement("b") != NULL ? lightElement->FirstChildElement("b")->FloatText() : 0.0f);
		}
		lightElement = element->FirstChildElement("diffuse");
		if (lightElement != NULL)
		{
			light.diffuse = glm::vec3(lightElement->FirstChildElement("r") != NULL ? lightElement->FirstChildElement("r")->FloatText() : 0.0f, lightElement->FirstChildElement("g") != NULL ? lightElement->FirstChildElement("g")->FloatText() : 0.0f, lightElement->FirstChildElement("b") != NULL ? lightElement->FirstChildElement("b")->FloatText() : 0.0f);
		}
		lightElement = element->FirstChildElement("specular");
		if (lightElement != NULL)
		{
			light.specular = glm::vec3(lightElement->FirstChildElement("r") != NULL ? lightElement->FirstChildElement("r")->FloatText() : 0.0f, lightElement->FirstChildElement("g") != NULL ? lightElement->FirstChildElement("g")->FloatText() : 0.0f, lightElement->FirstChildElement("b") != NULL ? lightElement->FirstChildElement("b")->FloatText() : 0.0f);
		}
		gameScreen->addLight(light);
	}

	static EnumParser<OnClickFunctions::FunctionType> enumParser;

};
EnumParser<OnClickFunctions::FunctionType> LevelLoader::enumParser;
#endif // !LEVELLOADER_H

