#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <gl\glm\glm\glm.hpp>
#include <gl\glm\glm\gtc\quaternion.hpp>


class Transform {
public:
	Transform() {
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		orientation = glm::quat();
	};
	Transform(glm::vec3 p, glm::vec3 s, glm::quat o) {
		position = p;
		scale = s;
		orientation = o;
	};
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat orientation;
};

#endif // !TRANSFORM_H
