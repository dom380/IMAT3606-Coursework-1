#include "Input.h"

bool Input::initialised = false;
Input Input::instance;

Input& Input::getInstance()
{
	if (initialised) {
		return instance;
	}
	instance = Input();
	return instance;
}

void Input::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	double cursorPosX, cursorPosY;
	glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		Event e(EventType::CLICK, cursorPosX, cursorPosY);
		for (shared_ptr<EventListener> listener : getInstance().mouseSubs) {
			listener->handle(e);
		}
	}
	
}

void Input::keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	//do something
}

void Input::registerKeyListener(shared_ptr<EventListener> listener)
{
	getInstance().keySubs.push_back(listener);
}

void Input::registerMouseListener(shared_ptr<EventListener> listener)
{
	getInstance().mouseSubs.push_back(listener);
}

Input::~Input()
{
	keySubs.clear();
	mouseSubs.clear();
	//for (shared_ptr<EventListener> sub : keySubs) {
	//	sub.reset();
	//}
	//for (shared_ptr<EventListener> sub : mouseSubs) {
	//	sub.reset();
	//}
	initialised = false;
}

Input::Input()
{
	initialised = true;
}
