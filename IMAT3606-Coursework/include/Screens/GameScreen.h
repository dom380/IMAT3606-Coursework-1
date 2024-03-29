#pragma once
#ifndef GAMESCREEN_H
#define GAMESCREEN_H
#include <Screens\Screen.h>
#include <memory>
#include <Camera\Camera.h>
#include <Camera\PerspectiveCamera.h>
#include <Input.h>
using std::shared_ptr;
#include <vector>
using std::vector;
#include <Robot.h>
#include <GUI\TextBox.h>
#include <GameObject.h>
#include <Components\Message.h>
#include <Components\LocationMessage.h>
#include <Components\RenderMessage.h>
#include <AssetManager.h>
#ifndef NDEBUG
#include <utils\Timer.h>
#endif
/*
	The gameplay implementaion of the Screen Class. 
	Implements EventListener.
*/
class GameScreen : public Screen, public EventListener
{
public:
	/*
		Constructor
		shared_ptr<Graphics>& renderer, Pointer to the graphics system.
		shared_ptr<Input>& input, Pointer to the input system.
		shared_ptr<Camera> camera, Pointer to a Camera. Defaulted to PerspectiveCamera.
	*/
	GameScreen(shared_ptr<Graphics>& renderer, shared_ptr<Input>& input, shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>());
	~GameScreen() {};
	/*
		Update this screen's objects.
		double dt, Current time step.
	*/
	void update(double dt);
	/*
		Render this screen.
	*/
	void render();
	/*
		Resize this screen.
		int width, The new width.
		int height, The new height.
	*/
	void resize(int width, int height);
	/*
		Clean up resources.
	*/
	void dispose();
	/*
		Add a light.
	*/
	void addLight(Light light);
	/*
		Add a TextBox to the screen.
	*/
	void addTextBox(shared_ptr<TextBox> textbox);
	/*
		Add a GameObject
	*/
	void addGameObject(shared_ptr<GameObject> gameObj);
	/*
		Calls to the graphics system to update the lighting buffer.
		This method must be called after any change to the lighting.
	*/
	void updateLighting();
	/*
		Increments the score and updates the score text.
	*/
	void updateScore();

	/*
		Empty implementation of Mouse Event handeling
	*/
	void  handle(MouseEvent event);
	/*
		Switches the camera if 'C' pressed.
	*/
	void  handle(KeyEvent event);
private:
	vector<shared_ptr<GameObject>> gameObjects;
	vector<shared_ptr<TextBox>> textBoxes;
	shared_ptr<Input> input;
	shared_ptr<Graphics> renderer;
	int activeCamera = 0;
	vector<shared_ptr<Camera>> cameras;
	vector<Light> lights;
	unsigned int lightingBlockId = -1;
	unsigned int lightingBufferId = -1;
	shared_ptr<Robot> robot;
	int currentScore = 0;
#ifndef NDEBUG //If debugging display how long the frame took in ms.
	Timer timer;
	shared_ptr<TextBox> frameTime;
#endif
};

#endif // !GAMESCREEN_H

