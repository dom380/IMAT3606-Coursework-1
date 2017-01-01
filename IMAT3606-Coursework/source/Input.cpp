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

void Input::mouseMovementCallback(GLFWwindow * window, double xpos, double ypos)
{
	MouseEvent e(MouseEventType::MOUSE_MOVE, xpos, ypos);
	vector<shared_ptr<EventListener>> listeners = getInstance().mouseSubs;
	for (shared_ptr<EventListener> listener : listeners) {
		listener->handle(e);
	}
}

void Input::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	double cursorPosX, cursorPosY;
	glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		MouseEvent e(MouseEventType::LEFT_CLICK, cursorPosX, cursorPosY);
		vector<shared_ptr<EventListener>> listeners = getInstance().mouseSubs;
		for (shared_ptr<EventListener> listener : listeners) {
			listener->handle(e);
		}
	}
	
}

void Input::keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	KeyEventType type;
	action == GLFW_PRESS ? type = KeyEventType::KEY_PRESSED : type = KeyEventType::KEY_RELEASED;
	KeyEvent e(type, key, mods);
	vector<shared_ptr<EventListener>> listeners = getInstance().keySubs;
	for (shared_ptr<EventListener> listener : listeners) {
		listener->handle(e);
	}
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
	initialised = false;
}

Input::Input()
{
	initialised = true;
}
