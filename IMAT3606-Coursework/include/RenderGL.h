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

class RenderGL : public Graphics {
public:
	RenderGL(int width, int height);
	~RenderGL();
	bool init();
	void prepare();
	void exit();
	int getWidth();
	int getHeight();
	void buildTextShader(unsigned int &vertArrayObj, unsigned int &vertBuffObj, shared_ptr<Shader> &textShader);
	void renderText(string& text, Font& font, Transform& transform, unsigned int VAO, unsigned int VBO, shared_ptr<Shader>& textShader);
	void buildFontTexture(FT_Face& fontFace, unsigned int& textureID);
	void bufferModelData(vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<glm::vec2>& textures,
		vector<unsigned short>& indices, unsigned int& vaoHandle);
	void bufferLightingData(vector<Light>& lights, shared_ptr<Shader> &shader, unsigned int& uniformBuffer,
		unsigned int& bindingPoint);
	void renderModel(Model& model, shared_ptr<Shader>& shaderProgram, shared_ptr<Camera>& camera);
	void renderModel(Model& model, shared_ptr<Shader>& shaderProgram, shared_ptr<Camera>& camera, vector<Light>& lights);
	void renderModel(Model& model, shared_ptr<Shader>& shaderProgram, shared_ptr<Camera>& camera, unsigned int lightingBuffer, unsigned int lightingBlockId);

private:
	int width;
	int height;
	unsigned int currBindingPoint = 0;
	glm::mat4 modelMat = glm::mat4();
	glm::mat4 viewMat = glm::mat4();
	glm::mat4 perspectiveMat = glm::mat4();
};

#endif // !RENDERGL_H

