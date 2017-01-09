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
	switch (action)
	{
	case GLFW_PRESS:
		type = KeyEventType::KEY_PRESSED;
		break;
	case GLFW_REPEAT:
		type = KeyEventType::KEY_REPEATED;
		break;
	case GLFW_RELEASE:
		type = KeyEventType::KEY_RELEASED;
		break;
	default:
		type = KeyEventType::KEY_PRESSED;
		break;
	}
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

void Input::removeKeyListener(shared_ptr<EventListener> listener)
{
	auto it = std::find(getInstance().keySubs.begin(), getInstance().keySubs.end(), listener);
	if (it != getInstance().keySubs.end())
	{
		getInstance().keySubs.erase(it);
	}
}

void Input::removeMouseListener(shared_ptr<EventListener> listener)
{
	auto it = std::find(getInstance().mouseSubs.begin(), getInstance().mouseSubs.end(), listener);
	if (it != getInstance().mouseSubs.end())
	{
		getInstance().mouseSubs.erase(it);
	}
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
