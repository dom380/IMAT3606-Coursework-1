#pragma once
#ifndef RENDERGL_H
#define RENDERGL_H
#include <gl\glew.h>
#include "GL\glm\glm\gtc\matrix_transform.hpp"
#include "GL\glm\glm\gtx\transform.hpp"
#include <string>
#include "Shader.h"
#include "Model.h"
#include <vector>
using std::string; using std::vector;
#include <memory>
using std::shared_ptr;

class RenderGL {
public:
	RenderGL(int width, int height);
	~RenderGL();
	void init();
	void render();
	Shader getShader();
	void setShader(Shader shader);
	void addModel(shared_ptr<Model> &model);
	void setModels(vector<shared_ptr<Model>> &modelList);
private:
	int width;
	int height;
	Shader shaderProg;
	vector<shared_ptr<Model>> models;

	float angle;
	glm::mat4 model = glm::mat4();
};

#endif // !RENDERGL_H

