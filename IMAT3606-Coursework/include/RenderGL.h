#pragma once
#ifndef RENDERGL_H
#define RENDERGL_H
#include "Graphics.h"
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

#include <GUI\Button.h>
#include <GUI\OnClickTest.h>

class RenderGL : public Graphics {
public:
	RenderGL(int width, int height);
	~RenderGL();
	bool init();
	void render();
	void exit();
	void addModel(shared_ptr<Model> &model);
	void setModels(vector<shared_ptr<Model>> &modelList);
	int getWidth();
	int getHeight();
	void buildTextShader(unsigned int &vertArrayObj, unsigned int &vertBuffObj, Shader &textShader);
	void renderText(string& text, Font& font, Transform& transform, unsigned int VAO, unsigned int VBO, Shader& textShader);
	void buildFontTexture(FT_Face& fontFace, unsigned int& textureID);
	void bufferModelData(vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<glm::vec2>& textures,
		vector<unsigned short>& indices, unsigned int& vaoHandle);

	void renderModel(Model& model, Shader& shaderProgram);
	//todo remove
	shared_ptr<Button> buttonTest;

private:
	int width;
	int height;
	//Shader shaderProg;
	vector<shared_ptr<Model>> models;
	float angle;
	glm::mat4 modelMat = glm::mat4();
	glm::mat4 viewMat = glm::mat4();
	glm::mat4 perspectiveMat = glm::mat4();

};

#endif // !RENDERGL_H

