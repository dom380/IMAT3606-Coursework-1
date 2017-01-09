#pragma once
#ifndef LEVELLOADER_H
#define LEVELLOADER_H
#include <Engine.h>
#include <Graphics.h>
#include <GUI\MenuScreen.h>
#include <GameScreen.h>
#include "OnClickFunctions.h"
#include "tinyxml2.h"
#ifndef NDEBUG
	#include "Timer.h"
#endif
/*
	Factory class to parse level XML descriptors and create the relevant objects.
	Calling these methods on a separate thread using OpenGL implementation requires a second 
	context and all Vertex Array Objects to be rebuild on the rendering thread.
*/
class LevelLoader {
public:
	/*
		Read the specified XML file then load the requested objects and register the screen with the engine.

		Returns false if an error occured.
	*/
	static bool loadLevel(Engine* engine, shared_ptr<Graphics> renderer, const char* filePath) 
	{
		try
		{
			tinyxml2::XMLDocument doc;
			doc.LoadFile(filePath);
			tinyxml2::XMLElement* screenElement = doc.FirstChildElement("screen");
			const char* type = screenElement->Attribute("type");
			if (string(type) == string("menu")) {
				return loadMenu(engine, renderer, screenElement);
			}
			else if (string(type) == string("level")) {
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
	/*
		Utility method to load MenuScreens
		Returns true on success.
	*/
	static bool loadMenu(Engine* engine, shared_ptr<Graphics>& renderer, tinyxml2::XMLElement* screenElement)
	{
		shared_ptr<MenuScreen> menuScreen = std::make_shared<MenuScreen>(renderer, engine);
		menuScreen->setID(screenElement->Attribute("name"));
		tinyxml2::XMLElement* stringElement = screenElement->FirstChildElement("strings");
		if (stringElement != NULL) stringElement = stringElement->FirstChildElement();
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

	/*
		Utility method to load Text elements
	*/
	static void loadStringElement(shared_ptr<Graphics>& renderer, shared_ptr<Screen> screen, tinyxml2::XMLElement* stringElement)
	{
		Font font = *AssetManager::getInstance()->getFont("./resources/fonts/arial.ttf", renderer);
		const char* text = stringElement->FirstChildElement("value")!=NULL ? stringElement->FirstChildElement("value")->GetText() : "MISSING_STRING";
		Transform transform;
		loadTransform(transform, stringElement);
		glm::vec3 colour;
		stringElement->FirstChildElement("colour") != NULL ? loadColour(colour, stringElement->FirstChildElement("colour"), glm::vec3(1.0, 1.0, 1.0)) : colour = glm::vec3(1.0, 1.0, 1.0);
		string id;
		stringElement->Attribute("id") != NULL ? id = stringElement->Attribute("id") : id = "";
		shared_ptr<TextBox> textBox = std::make_shared<TextBox>(text, font, transform, renderer, colour, id);
		screen->addTextBox(textBox);
	}

	/*
		Utility method to load Button elements
	*/
	static void loadButtonElement(Engine* engine, shared_ptr<Graphics>& renderer, shared_ptr<MenuScreen> menuScreen, tinyxml2::XMLElement* buttonElement)
	{
		Font font = *AssetManager::getInstance()->getFont("./resources/fonts/arial.ttf", renderer);
		const char* text = buttonElement->FirstChildElement("value")!=NULL ? buttonElement->FirstChildElement("value")->GetText() : "MISSING_STRING";
		Transform transform;
		loadTransform(transform, buttonElement);
		glm::vec3 colour;
		buttonElement->FirstChildElement("colour") != NULL ? loadColour(colour, buttonElement->FirstChildElement("colour"), glm::vec3(1.0, 1.0, 1.0)) : colour = glm::vec3(1.0, 1.0, 1.0);
		string id;
		buttonElement->Attribute("id") != NULL ? id = buttonElement->Attribute("id") : id = "";
		shared_ptr<Button> button = std::make_shared<Button>(text, font, transform, renderer, colour, id);
		menuScreen->addButton(button);
		Input::getInstance().registerMouseListener(button);
		string funcName = string(buttonElement->FirstChildElement("function")->Attribute("type"));
		loadButtonFunc(funcName, buttonElement, button, engine);
	}

	/*
		Utility method to bind the supplied parameters to the requested callback function.
	*/
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

	/*
		Utility method to load GameScreens
		Returns true on sucess.
	*/
	static bool loadGameLevel(Engine* engine, shared_ptr<Graphics>& renderer, tinyxml2::XMLElement* screenElement) 
	{
#ifndef NDEBUG
			Timer timer;
			timer.start();
#endif
		shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(engine->getWindowWidth(), engine->getWindowHeight(), 45.f);
		shared_ptr<GameScreen> gameScreen = std::make_shared<GameScreen>(renderer, camera);
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
		tinyxml2::XMLElement* stringElement = screenElement->FirstChildElement("strings");
		if (stringElement != NULL) stringElement = stringElement->FirstChildElement();
		while (stringElement != NULL) {
			loadStringElement(renderer, gameScreen, stringElement);
			stringElement = stringElement->NextSiblingElement();
		}
		gameScreen->updateLighting();
		engine->registerScreen(gameScreen);
		Input::getInstance().registerKeyListener(gameScreen);
#ifndef NDEBUG
		double time = timer.getElapsedTime();
#endif
		return true;
	}

	/*
		Utility method to load a Model.

		Currently only supports Wavefront .obj format.
	*/
	static void loadModel(shared_ptr<Graphics>& renderer, shared_ptr<GameScreen> gameScreen, tinyxml2::XMLElement* modelElement)
	{
		shared_ptr<Model> model = std::make_shared<Model>(renderer);
		const char* modelPath = modelElement->FirstChildElement("file")->GetText();
		const char* texturePath = modelElement->FirstChildElement("texture")!=NULL ? modelElement->FirstChildElement("texture")->GetText():NULL;
		string id;
		modelElement->Attribute("id") != NULL ? id = modelElement->Attribute("id") : id = "";
		model->init(modelPath, texturePath, id);
		loadTransform(model->transform, modelElement);
		gameScreen->addModel(model);
	}

	/*
		Utility method to load Transform objects
	*/
	static void loadTransform(Transform &transform, tinyxml2::XMLElement* element) 
	{
		glm::vec3 pos;
		glm::vec3 scale = glm::vec3(1.0,1.0,1.0);
		glm::quat quat; quat.y = 1.0f; quat.w = 0.0f;
		tinyxml2::XMLElement* posElement = element->FirstChildElement("position");
		if (posElement != NULL) {
			pos = glm::vec3
				(
					posElement->FirstChildElement("x")!=NULL ? posElement->FirstChildElement("x")->FloatText():0.0f, 
					posElement->FirstChildElement("y")!=NULL ? posElement->FirstChildElement("y")->FloatText():0.0f, 
					posElement->FirstChildElement("z")!=NULL ? posElement->FirstChildElement("z")->FloatText() : 0.0f
				);
		}
		tinyxml2::XMLElement* scaleElement = element->FirstChildElement("scale");
		if (scaleElement != NULL) {
			scale = glm::vec3
				(
					scaleElement->FirstChildElement("x")!=NULL ? scaleElement->FirstChildElement("x")->FloatText(1.0f) : 1.0f, 
					scaleElement->FirstChildElement("y") != NULL ? scaleElement->FirstChildElement("y")->FloatText(1.0f) : 1.0f, 
					scaleElement->FirstChildElement("z") != NULL ? scaleElement->FirstChildElement("z")->FloatText(1.0f) : 1.0f
				);
		}
		tinyxml2::XMLElement* quatElement = element->FirstChildElement("orientation");
		if (quatElement != NULL) {
			quat = glm::quat
				(
					quatElement->FirstChildElement("w") != NULL ? quatElement->FirstChildElement("w")->FloatText() : 0.0f,
					quatElement->FirstChildElement("x")!=NULL ? quatElement->FirstChildElement("x")->FloatText() : 0.0f, 
					quatElement->FirstChildElement("y")!=NULL ? quatElement->FirstChildElement("y")->FloatText(1.0f) : 1.0f, //default to be orientated around y axis
					quatElement->FirstChildElement("z")!=NULL ? quatElement->FirstChildElement("z")->FloatText() : 0.0f
				);
		}
		transform.orientation = quat;
		transform.position = pos;
		transform.scale = scale;
	}

	/*
		Utility method to load Light objcts
	*/
	static void loadLight(shared_ptr<GameScreen> gameScreen, tinyxml2::XMLElement* element)
	{
		Light light = Light();
		tinyxml2::XMLElement* posElement = element->FirstChildElement("position");
		if (posElement != NULL) 
		{
			light.pos = glm::vec3
				(
					posElement->FirstChildElement("x") != NULL ? posElement->FirstChildElement("x")->FloatText() : 0.0f, 
					posElement->FirstChildElement("y") != NULL ? posElement->FirstChildElement("y")->FloatText() : 0.0f, 
					posElement->FirstChildElement("z") != NULL ? posElement->FirstChildElement("z")->FloatText() : 0.0f
				);
		}
		tinyxml2::XMLElement* lightElement = element->FirstChildElement("ambient");
		if (lightElement != NULL)
		{
			loadColour(light.ambient, lightElement);
		}
		lightElement = element->FirstChildElement("diffuse");
		if (lightElement != NULL)
		{
			loadColour(light.diffuse, lightElement);
		}
		lightElement = element->FirstChildElement("specular");
		if (lightElement != NULL)
		{
			loadColour(light.specular, lightElement);
		}
		gameScreen->addLight(light);
	}

	/*
		Utility method to load Colour objcts
	*/
	static void loadColour(glm::vec3& colour, tinyxml2::XMLElement* element, glm::vec3 defaultVal = glm::vec3(0.0,0.0,0.0))
	{
		colour = glm::vec3
			(
				element->FirstChildElement("r") != NULL ? element->FirstChildElement("r")->FloatText() : defaultVal.r, 
				element->FirstChildElement("g") != NULL ? element->FirstChildElement("g")->FloatText() : defaultVal.g, 
				element->FirstChildElement("b") != NULL ? element->FirstChildElement("b")->FloatText() : defaultVal.b
			);
	}

	static EnumParser<OnClickFunctions::FunctionType> enumParser;

};
EnumParser<OnClickFunctions::FunctionType> LevelLoader::enumParser;
#endif // !LEVELLOADER_H

