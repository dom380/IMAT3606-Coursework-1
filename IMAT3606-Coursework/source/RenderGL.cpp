#include "RenderGL.h"

RenderGL::RenderGL(int width, int height)
{
	this->width = width;
	this->height = height;
}

RenderGL::~RenderGL()
{
	for (shared_ptr<Model> model : models) {
		model.reset();
	}
}

void RenderGL::init()
{
	glEnable(GL_DEPTH_TEST);
	shaderProg.compileShader("./shaders/basic.vert", GL_VERTEX_SHADER);
	shaderProg.compileShader("./shaders/basic.frag", GL_FRAGMENT_SHADER);
	shaderProg.link();
	shaderProg.bindShader();
	model = glm::translate(model, glm::vec3(0.0f, 0.0f,0.0f));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 perspective = glm::perspective(glm::radians(45.f), (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
	shaderProg.setUniform("mModel", model);
	shaderProg.setUniform("mView", view);
	shaderProg.setUniform("mProjection", perspective);
}

void RenderGL::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	angle++;
	//if (angle>360.f)
	//{
	//	angle -= 360;
	//}
	//glm::mat4 modelV = glm::rotate(glm::radians(angle),glm::vec3(1.0f,0.0f,0.0f)) *model;
	//shaderProg.setUniform("mModel", modelV);
	for (shared_ptr<Model> model : models) {
		model->render(shaderProg);
	}
}

Shader RenderGL::getShader()
{
	return shaderProg;
}

void RenderGL::setShader(Shader shader)
{
	shaderProg = shader;
}

void RenderGL::addModel(shared_ptr<Model> &model)
{
	models.push_back(model);
}

void RenderGL::setModels(vector<shared_ptr<Model>>& modelList)
{
	models = modelList;
}
