#pragma once
#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H
#define GLM_FORCE_RADIANS

#include "Camera.h"
#include <gl\glm\glm\gtc\matrix_transform.hpp>
#include <gl\glm\glm\glm.hpp>
#include <gl\glm\glm\gtc\type_ptr.hpp>

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(glm::vec3 pos = glm::vec3(0.0, 0.0, 1.0), glm::vec3 up = glm::vec3(0.0, 1.0, 0.0), glm::vec3 direction = glm::vec3(0.0, 0.0, -1.0));
	void update();
	void lookAt(float x, float y, float z);
	void lookAt(glm::vec3 target);
	void handle(MouseEvent event);
	void handle(KeyEvent event);
protected:
	glm::vec3 worldY = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 worldX = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 worldZ = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::quat orientation;

private:
	glm::quat getRotation(glm::vec3 axis, float angle);
	int previousX, previousY;

};

#endif