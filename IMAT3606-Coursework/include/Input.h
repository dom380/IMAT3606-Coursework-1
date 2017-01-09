#pragma once
#ifndef INPUT_H
#define INPUT_H
#include <gl\glfw3.h>
#include <vector>
using std::vector;
#include "GUI\EventListener.h"
#include "GUI\MouseEvent.h"
#include <memory>
using std::shared_ptr;

/*
	This should probably be abstract and have concrete impls for each windowing systems.
	But for now just sitck with GLFW.
*/
class Input {
public:
	static Input& getInstance();
	static void mouseMovementCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void registerKeyListener(shared_ptr<EventListener> listener);
	static void registerMouseListener(shared_ptr<EventListener> listener);
	static void removeKeyListener(shared_ptr<EventListener> listener);
	static void removeMouseListener(shared_ptr<EventListener> listener);

	~Input();
private:
	static Input instance;
	static bool initialised;
	vector<shared_ptr<EventListener>>keySubs;
	vector<shared_ptr<EventListener>>mouseSubs;
	Input();
	Input(Input const&) {}; // prevent copies
	void operator=(Input const&) {}; // prevent assignments
};

#endif // !INPUT_H
