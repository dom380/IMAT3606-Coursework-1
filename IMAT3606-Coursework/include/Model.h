#pragma once
#ifndef MODEL_H
#define MODEL_H
#include "utils\stb_image.h"
#include "utils\Bitmap.h"
#include "utils\Texture.h"
#include "utils\ObjReader.h"
#include "GL\glew.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "GL\glm\glm\glm.hpp"
#include "Shader.h"
#include <vector>
using std::vector;

class Model {
public:
	Model();
	~Model();
	void init(char* objFile, char* textureFile);
	void render(Shader &shaderProgram);
private:
	bool initalised = false;

	GLuint verticesId;
	GLuint texCoordsId;
	GLuint normalsId;
	GLuint indexId;
	GLuint textureId;

	GLsizei indexSize;

	GLuint vboHandles[4];
	GLuint vaoHandle;

	Texture* texture;
};
#endif // !MODEL_H
