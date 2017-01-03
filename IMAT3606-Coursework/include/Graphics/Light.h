#pragma once
#ifndef LIGHT_H
#define LIGHT_H
#include <gl\glm\glm\glm.hpp>

class Light
{
public:
	Light() {
		pos = glm::vec3(0.0,0.0,0.0);
		ambient = glm::vec3(0.0, 0.0, 0.0);
		diffuse = glm::vec3(0.0, 0.0, 0.0);
		specular = glm::vec3(0.0, 0.0, 0.0);
	};
	Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool enabled = false)
	{
		this->pos = position;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->enabled = enabled;
	};
	glm::vec3 pos;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	bool enabled = false;
};

#endif // !LIGHT_H
