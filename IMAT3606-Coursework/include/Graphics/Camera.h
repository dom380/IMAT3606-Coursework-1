#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include <gl\glm\glm\glm.hpp>
#include <GUI\EventListener.h>

class Camera : public EventListener
{
public:
	virtual void lookAt(float x, float y, float z) = 0;
	virtual void lookAt(glm::vec3 target) = 0;
	virtual void move(glm::vec3 newPos)
	{
		position = newPos;
		update();
	}
	virtual void move(float x, float y, float z)
	{
		position = glm::vec3(x,y,z);
		update();
	}
	glm::mat4 getView()
	{
		return view;
	}
	glm::vec3 getPosition()
	{
		return position;
	}
	enum CameraClass
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

protected:
	virtual void update() = 0;

	glm::mat4 view;
	glm::vec3 frontDir;
	glm::vec3 upDir;
	glm::vec3 rightDir;
	glm::vec3 position;
	glm::mat4 projection;
	CameraClass type;

};
#endif // !CAMERA_H
