#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <Shader.h>
#include <GUI\Font.h>
#include <utils\Transform.h>
class Font;
class Graphics
{
public:
	virtual void render() = 0;
	virtual bool init() = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual void buildTextShader(unsigned int &vertArrayObj, unsigned int &vertBuffObj, Shader &textShader) = 0;
	//todo clear up this method so we don't need to pass so many params
	virtual void buildFontTexture(FT_Face& fontFace, unsigned int& textureID) = 0;
	virtual void renderText(string& text, Font& font, Transform& transform, unsigned int VAO, unsigned int VBO, Shader& textShader) = 0;
	virtual void renderModel(Model model);
	virtual void exit() = 0;
};

#endif // !GRAPHICS_H
