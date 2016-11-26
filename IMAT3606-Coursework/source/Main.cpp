#include "RenderGL.h"
#include <GL/glew.h>
#include "GL/glfw3.h"
#include <cstdlib>
#include <stdio.h>


#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1024
GLFWwindow *window;
RenderGL renderer = RenderGL(WINDOW_WIDTH, WINDOW_HEIGHT);
shared_ptr<Model> modelTest = std::make_shared<Model>();

// Main loop 
void mainLoop() {
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		renderer.render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

int main() {
	
	// Initialize GLFW
	if (!glfwInit()) exit(EXIT_FAILURE);

	//	 Select OpenGL 4.3 with a forward compatible core profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_DOUBLEBUFFER, true);

	// Open the window
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "IMAT3606-CourseWork", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	//glfwSetKeyCallback(window, key_callback);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	}
	

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//modelTest->init("./cube.obj", "./texture.bmp");
	modelTest->init("./cube.obj", "./texture.bmp");
	renderer.addModel(modelTest);
	renderer.init();
	string check = OpenGLSupport().GetError();
	mainLoop();

	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	exit(EXIT_SUCCESS);

}