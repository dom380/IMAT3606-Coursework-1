#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <Shader.h>
#include <memory>
#include <Graphics\Camera.h>
using std::shared_ptr;
#include <GUI\Font.h>
#include <Graphics\Transform.h>
#include <vector>
using std::vector;
class Font;
class Model;
class Graphics
{
public:
	/*
		Method called prior to rendering each frame. 
		Implementations can use this for per frame initialisation 
		e.g. clearing colour buffers etc.
	*/
	virtual void prepare() = 0;
	/*	
		Initialisation method called on start up.
		Implementations should perform one off and time consuming initalisation here.
	*/
	virtual bool init() = 0;
	/*
		Return the current window width.
	*/
	virtual int getWidth() = 0;
	/*
		Return the current window height.
	*/
	virtual int getHeight() = 0;
	/*	
		Build the requested model vertex buffer(s) 
	*/
	virtual void bufferModelData(vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<glm::vec2>& textures, 
		vector<unsigned short>& indices, unsigned int& vaoHandle) = 0;
	/*	
		Initialise a shader for text rendering.
	*/
	virtual void buildTextShader(unsigned int &vertArrayObj, unsigned int &vertBuffObj, Shader &textShader) = 0;
	/*
		Build the texture data for the requested freetype2 font face.
	*/
	virtual void buildFontTexture(FT_Face& fontFace, unsigned int& textureID) = 0;
	/*	
		Render text.
	*/
	virtual void renderText(string& text, Font& font, Transform& transform, unsigned int VAO, unsigned int VBO, Shader& textShader) = 0;
	/*
		Render model.
	*/
	virtual void renderModel(Model& model, Shader& shaderProgram, shared_ptr<Camera>& camera) = 0;

	/*
		Method called on engine shutdown. 
		Perform any further clean up of resources here.
	*/
	virtual void exit() = 0;

};

#endif // !GRAPHICS_H
