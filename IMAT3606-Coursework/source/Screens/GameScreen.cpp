#include "Screens\GameScreen.h"

GameScreen::GameScreen(shared_ptr<Graphics>& renderer, shared_ptr<Input>& input, shared_ptr<Camera> camera) :
	robot(std::make_shared<Robot>(AssetManager::getInstance()->getShader(std::pair<string, string>("colour.vert", "colour.frag"))))
{
	this->renderer = renderer;
	camera->move(58.0, 41.0f, 68.0f);
	camera->lookAt(glm::vec3(-1.0,-0.6,-1.0));
	cameras.push_back(camera);
	cameras.push_back(std::make_shared<PerspectiveCamera>(renderer->getWidth(), renderer->getHeight(), 45.0f, glm::vec3(-58,41,-68),glm::vec3(0,1,0), glm::vec3(0.63,-0.36,0.67)));
	cameras.push_back(std::make_shared<PerspectiveCamera>(renderer->getWidth(), renderer->getHeight(), 45.0f, glm::vec3(-58, 41, 68), glm::vec3(0, 1, 0), glm::vec3(0.59,-0.39,-0.7)));
	cameras.push_back(std::make_shared<PerspectiveCamera>(renderer->getWidth(), renderer->getHeight(), 45.0f, glm::vec3(58, 42, -68), glm::vec3(0, 1, 0), glm::vec3(-0.8,-0.42,0.42)));
	shared_ptr<PerspectiveCamera> robotCam = std::make_shared<PerspectiveCamera>(renderer->getWidth(), renderer->getHeight(), 45.0f, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1));
	cameras.push_back(robotCam);
	robot->setCamera(robotCam);
#ifndef NDEBUG
	glm::quat quat; quat.y = 1.0f; quat.w = 0.0f;
	shared_ptr<Transform> textPos = std::make_shared<Transform>(glm::vec3(30, 30, 0), glm::vec3(0.5, 0.5, 0.5), quat);
	frameTime = std::make_shared<TextBox>("Frame Time: 0", *AssetManager::getInstance()->getFont("arial.ttf", renderer), textPos, renderer);
	textBoxes.push_back(frameTime);
#endif
	this->input = input;
	this->input->registerKeyListener(robot);
	//this->registerKeyListener(cameras.at(0));
	this->input->registerMouseListener(robotCam);
	activeCamera = 0;
}

void GameScreen::update(double dt)
{
#ifndef NDEBUG
	timer.start();
#endif
	robot->Prepare(dt);
	Message* robotLocMsg = new LocationMessage(robot->getPosition());
	for (shared_ptr<GameObject> gameObj : gameObjects) {
		gameObj->updateComponents(dt);
		gameObj->notifyAll(robotLocMsg);
	}
	delete robotLocMsg;
}

void GameScreen::render()
{
	shared_ptr<Camera> camera = cameras.at(activeCamera);
	robot->DrawRobot(camera->getView(), camera->getProjection());
	Message* renderMsg = new RenderMessage(camera, lightingBufferId, lightingBlockId);
	for (shared_ptr<GameObject> gameObj : gameObjects) {
		gameObj->notifyAll(renderMsg);
	}
	delete renderMsg;
	for (shared_ptr<TextBox> textBox : textBoxes)
	{
		textBox->render();
	}
#ifndef NDEBUG
	double elapsedTime = timer.getElapsedTimeMilliSec();
	string frameText = "Frame Time: " + std::to_string( elapsedTime ) + "ms";
	frameTime->updateText(frameText);
	timer.stop();
#endif
}

void GameScreen::resize(int width, int height)
{
}

void GameScreen::dispose()
{
	for (shared_ptr<Camera> camera : cameras) {
		input->removeMouseListener(camera);
		input->removeKeyListener(camera);
	}
	input->removeKeyListener(robot);
	gameObjects.clear();
	lights.clear();
	robot.reset();
	cameras.clear();
}

void GameScreen::addLight(Light light)
{
	lights.push_back(light);
}

void GameScreen::addTextBox(shared_ptr<TextBox> textbox)
{
	textBoxes.push_back(textbox);
}

void GameScreen::addGameObject(shared_ptr<GameObject> gameObj)
{
	gameObjects.push_back(gameObj);
}

void GameScreen::updateLighting()
{
	std::shared_ptr<Shader>shader = AssetManager::getInstance()->getShader(std::pair<string, string>("phong.vert", "phong.frag"));
	renderer->bufferLightingData(lights, shader, lightingBufferId, lightingBlockId);
}

void GameScreen::handle(MouseEvent event)
{
	//NOP
}

void GameScreen::handle(KeyEvent event)
{
	if (event.type == KeyEventType::KEY_PRESSED)
	{
		if (event.key == 67)//c
		{
			activeCamera++;
			if (activeCamera >= cameras.size()) activeCamera = 0;
		}
	}
}

void GameScreen::updateScore()
{
	currentScore++;
	for (shared_ptr<TextBox> textbox : textBoxes)
	{
		if (textbox->getId() == string("score_string"))
		{
			textbox->updateText("Gold Collected: " + std::to_string(currentScore));
		}
	}
}
