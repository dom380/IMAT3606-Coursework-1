#include "RenderGL.h"
#ifndef NDEBUG
#include <utils\GLSupport.h>
#endif


RenderGL::RenderGL(int width, int height)
{
	this->width = width;
	this->height = height;
}

RenderGL::~RenderGL()
{
	
}

bool RenderGL::init()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return false;

	}
	glEnable(GL_DEPTH_TEST);
	shaderProg.compileShader("./shaders/basic.vert", GL_VERTEX_SHADER);
	shaderProg.compileShader("./shaders/basic.frag", GL_FRAGMENT_SHADER);
	shaderProg.link();
	shaderProg.bindShader();
	model = glm::translate(model, glm::vec3(0.0f, 0.0f,0.0f));
	//todo move these to camera class?
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 perspective = glm::perspective(glm::radians(45.f), (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
	shaderProg.setUniform("mModel", model);
	shaderProg.setUniform("mView", view);
	shaderProg.setUniform("mProjection", perspective);

	//todo remove test code:

	string text("Button_test");
	FT_Library ft;
	FT_Error error = FT_Init_FreeType(&ft);
	Font font(ft, "./resources/fonts/arial.ttf", this);
	font.compile();
	Transform transform(glm::vec3(30.0,30.0,-1.0), glm::vec3(1.0,1.0,1.0), glm::quat());
	buttonTest = std::make_shared<Button<OnClickTest>>(text, font, transform, this);

	return true;
}

void RenderGL::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderProg.bindShader();
	angle++;
	if (angle>360.f)
	{
		angle -= 360;
	}
	glm::mat4 modelV = glm::rotate(glm::radians(angle),glm::vec3(1.0f,1.0f,1.0f)) *model;
	shaderProg.setUniform("mModel", modelV);
	//TODO Move list of models out of renderer
	for (shared_ptr<Model> model : models) {
		model->render(shaderProg);
	}
	buttonTest->render();
}

void RenderGL::exit() {
	for (shared_ptr<Model> model : models) {
		model.reset();
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

int RenderGL::getWidth()
{
	return width;
}

int RenderGL::getHeight()
{
	return height;
}

void RenderGL::buildTextShader(unsigned int &vertArrayObj, unsigned int &vertBuffObj, Shader & textShader)
{
	glGenVertexArrays(1, &vertArrayObj);
	glGenBuffers(1, &vertBuffObj);
	glBindVertexArray(vertArrayObj);
	//create buffer for vertex and texture data
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	textShader.compileShader("./shaders/text.vert", GL_VERTEX_SHADER);
	textShader.compileShader("./shaders/text.frag", GL_FRAGMENT_SHADER);
	textShader.link();
	textShader.bindShader();
	glm::mat4 projection = glm::ortho(0.0f, 1024.0f, 0.0f, 800.0f);
	textShader.setUniform("projection", projection);
	textShader.setUniform("textColour", glm::vec3(1.0, 1.0, 1.0));
	textShader.setUniform("tex", 0);
}

void RenderGL::renderText(string& text, Font& font, Transform& transform, unsigned int VAO, unsigned int VBO, Shader& textShader)
{

	float charX = transform.position.x;
	float charY = transform.position.y;
	glEnable(GL_BLEND);
#ifndef NDEBUG
	string check = OpenGLSupport().GetError();
#endif
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	textShader.bindShader();
#ifndef NDEBUG
	check = OpenGLSupport().GetError();
#endif
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	for (char c : text) {
		Font::Character character = font.getChar(c);
		GLfloat xpos = charX + character.bearing.x * transform.scale.x;
		GLfloat ypos = charY - (character.size.y - character.bearing.y) * transform.scale.y;

		GLfloat w = character.size.x * transform.scale.x;
		GLfloat h = character.size.y * transform.scale.y;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, character.texture);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		charX += (character.offset >> 6) *  transform.scale.x; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glDisable(GL_BLEND);
}

void RenderGL::buildFontTexture(FT_Face& fontFace, unsigned int& textureID)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Disable OpenGLs byte alignment restriction as Freetype uses 1 byte colour (grey scale)
	// Generate texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		fontFace->glyph->bitmap.width,
		fontFace->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		fontFace->glyph->bitmap.buffer
		);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void RenderGL::renderModel(Model model)
{
}
