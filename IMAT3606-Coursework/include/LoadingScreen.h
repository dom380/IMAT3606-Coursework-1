#pragma once
#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H
#include <Engine.h>
#include <Screen.h>
#include <GUI\TextBox.h>
#include <utils\LevelLoader.h>
#include <AssetManager.h>
#include <Graphics\Transform.h>
#include <memory>
#include <string>
#include <atomic>
#include <thread>
#include <future>

static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}

class LoadingScreen : public Screen
{
private:
	std::atomic<bool> loading = true;
	std::atomic<bool> succeded = false;
	std::thread backgroundThread;
	TextBox loadingText;
	Engine* gameEngine;
	std::string levelId;
	double time = 0.0;
public:

	LoadingScreen(GLFWwindow* context, Engine* engine, std::shared_ptr<Graphics> renderer, std::string levelToLoad)
	{
		Transform transform;
		transform.orientation.y = 1.0f; transform.orientation.w = 0.0f;
		transform.position.x = 30; transform.position.y = 500;
		loadingText = TextBox("Loading", *AssetManager::getInstance()->getFont("arial.ttf", renderer), transform, renderer);
		levelId = levelToLoad;
		gameEngine = engine;
		backgroundThread = std::thread([this, context, engine, renderer, levelToLoad] {
			glfwSetErrorCallback(error_callback);
			glfwMakeContextCurrent(context);
			string path = AssetManager::getInstance()->getRootFolder(AssetManager::ResourceType::LEVEL) + levelToLoad + ".xml";
			this->succeded = LevelLoader::loadLevel(engine, renderer, path.c_str());
			this->loading = false;
		});
	}
	void update(double dt)
	{
		if (!loading)
		{
			backgroundThread.join();
			if (succeded) {
				gameEngine->replaceScreen(levelId);
			} 
			else {
				gameEngine->replaceScreen(gameEngine->getInitialScreenId());
			}
		}
		time += dt;
		if (time > 1.0 && time < 2.0)
		{
			loadingText.updateText("Loading.");
		}
		else if (time > 2.0 && time < 3.0)
		{
			loadingText.updateText("Loading..");
		}
		else if (time > 3.0 && time < 4.0)
		{
			loadingText.updateText("Loading...");
		}
		else if (time > 4.0)
		{
			loadingText.updateText("Loading");
			time = 0;
		}
	}

	void render()
	{
		loadingText.render();
	}
	void resize(int width, int height) {};
	void dispose() {};
	void addTextBox(shared_ptr<TextBox> textbox) {};

};

#endif // !LOADINGSCREEN_H