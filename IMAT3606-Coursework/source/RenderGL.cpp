#include "RenderGL.h"
#ifndef NDEBUG
#include <utils\GLSupport.h>
#endif
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f,0.0f));
	//todo move these to camera class?
	viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return true;
}

void RenderGL::prepare()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//buttonTest->render();
}

void RenderGL::exit() {

}

int RenderGL::getWidth()
{
	return width;
}

int RenderGL::getHeight()
{
	return height;
}

void RenderGL::buildTextShader(unsigned int &vertArrayObj, unsigned int &vertBuffObj, shared_ptr<Shader> & textShader)
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
	textShader = AssetManager::getInstance()->getShader(std::pair<string, string>("./shaders/text.vert","./shaders/text.frag"));
#ifndef NDEBUG
	string check = OpenGLSupport().GetError();
#endif
	glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
	textShader->setUniform("projection", projection);
	textShader->setUniform("textColour", glm::vec3(1.0, 1.0, 1.0));
	textShader->setUniform("tex", 0);
	glFlush();
}

void RenderGL::renderText(string& text, Font& font, Transform& transform, unsigned int VAO, unsigned int VBO, shared_ptr<Shader>& textShader, glm::vec3 colour)
{
	float charX = transform.position.x;
	float charY = transform.position.y;
	glEnable(GL_BLEND);
#ifndef NDEBUG
	string check = OpenGLSupport().GetError();
#endif
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	textShader->bindShader();
	textShader->setUniform("textColour", colour);
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
#ifndef NDEBUG
	check = OpenGLSupport().GetError();
#endif
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
	glFlush();
}

vector<unsigned int> RenderGL::bufferModelData(vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<glm::vec2>& textures,
	vector<unsigned short>& indices, unsigned int& vaoHandle)
{
	unsigned int vboHandles[4];
	glGenBuffers(4, vboHandles);
	//Create IBO
	GLuint indiceBuffer = vboHandles[0];
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//Create VBO
	GLuint vertexBuffer = vboHandles[1];
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * vertices.size(), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Create VBO for textureCoords
	GLuint texCoordBuffer = vboHandles[2];
	if (textures.size() > 0) 
	{
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textures.size(), glm::value_ptr(textures[0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	//Creat VBO for normals
	GLuint normalBuffer = vboHandles[3];
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), glm::value_ptr(normals[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	vector<unsigned int> handles;
	handles.push_back(vertexBuffer);
	handles.push_back(texCoordBuffer);
	handles.push_back(normalBuffer);
	handles.push_back(indiceBuffer);
	glFlush();
	return handles;
}

void RenderGL::bufferLightingData(vector<Light>& lights, shared_ptr<Shader> &shader, unsigned int& uniformBuffer, unsigned int& bindingPoint)
{
	shader->bindShader();
	vector<glm::vec4> data;
	int numOfLights = 0;
	for (Light currLight : lights)
	{
		//GLSL pads vec3 as vec4 so when buffering store them as vec4.
		data.push_back(glm::vec4(currLight.pos, 1.0));
		data.push_back(glm::vec4(currLight.ambient, 1.0));
		data.push_back(glm::vec4(currLight.diffuse, 1.0));
		data.push_back(glm::vec4(currLight.specular, 1.0));
		numOfLights++;
		if (numOfLights >= 10) break; //Only support 10 lights
	}
	if (bindingPoint >= 0 && bindingPoint <= currBindingPoint) //If buffer has already been created, just update the data
	{
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * data.size(), glm::value_ptr(data[0]), GL_DYNAMIC_DRAW);
		return;
	}
	shader->bindUniformBlock("LightingBlock", currBindingPoint);
	glGenBuffers(1, &uniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * data.size(), glm::value_ptr(data[0]), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, currBindingPoint, uniformBuffer);
	glFlush();
	bindingPoint = currBindingPoint;
	currBindingPoint++;
}

unsigned int RenderGL::createVertexArrayObject(vector<unsigned int>& vboHandles)
{
	unsigned int vaoHandle;
	//Create Vertex Array Object
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//Bind VBOs to VAO
	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, (GLubyte *)NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, (GLubyte *)NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, (GLubyte *)NULL);
	//Bind IBO to VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[3]);
	return vaoHandle;
}

unsigned int RenderGL::createTextVertexArrayObject(unsigned int & vboHandle)
{
	unsigned int vertArrayObj;
	glGenVertexArrays(1, &vertArrayObj);
	glBindVertexArray(vertArrayObj);
	//create buffer for vertex and texture data
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	return vertArrayObj;
}

void RenderGL::renderModel(Model& model, shared_ptr<Shader>& shaderProgram, shared_ptr<Camera>& camera)
{
	vector<Light> defaultLights;
	defaultLights.push_back(Light());
	renderModel(model, shaderProgram, camera, defaultLights);
}

void RenderGL::renderModel(Model& model, shared_ptr<Shader>& shaderProgram, shared_ptr<Camera>& camera, vector<Light>& lights)
{
#ifndef NDEBUG
	string check = OpenGLSupport().GetError();
#endif
	shaderProgram->bindShader();
#ifndef NDEBUG
	check = OpenGLSupport().GetError();
#endif
	glBindVertexArray(model.getVertArray());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model.getTexture()->object());
	glm::quat orientation = model.transform.orientation;
	glm::mat4 mMat = modelMat * glm::translate(model.transform.position) * glm::rotate(orientation.w, glm::vec3(orientation.x, orientation.y, orientation.z)) * glm::scale(model.transform.scale);
	shaderProgram->setUniform("tex", 0);
	shaderProgram->setUniform("mView", camera->getView());
	shaderProgram->setUniform("mProjection", camera->getProjection());
	shaderProgram->setUniform("mModel", mMat);
	shaderProgram->setUniform("viewPos", camera->getPosition());
	if (model.getMaterial().used)
	{
		shaderProgram->setUniform("material", model.getMaterial());
	}
	if (lights.size() > 0)
	{
		shaderProgram->setUniform("lights", lights);
	}
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model.getIndexSize()), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
#ifndef NDEBUG
	check = OpenGLSupport().GetError();
#endif
	glBindVertexArray(0);
}

void RenderGL::renderModel(Model & model, shared_ptr<Shader>& shaderProgram, shared_ptr<Camera>& camera, unsigned int lightingBuffer, unsigned int lightingBlockId)
{
#ifndef NDEBUG
	string check = OpenGLSupport().GetError();
#endif
	shaderProgram->bindShader();
#ifndef NDEBUG
	check = OpenGLSupport().GetError();
#endif
	glBindVertexArray(model.getVertArray());
	if (model.getTexture() != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model.getTexture()->object());
		shaderProgram->setUniform("tex", 0);
	}
	else {
		shaderProgram = AssetManager::getInstance()->getShader(std::pair<std::string, std::string>("./shaders/phong_no_texture.vert", "./shaders/phong_no_texture.frag"));
		shaderProgram->bindShader();
	}
	glm::quat orientation = model.transform.orientation;
	glm::mat4 mMat = modelMat * glm::translate(model.transform.position) * glm::rotate(glm::radians(orientation.w), glm::vec3(orientation.x, orientation.y, orientation.z)) * glm::scale(model.transform.scale);
	shaderProgram->setUniform("mView", camera->getView());
	shaderProgram->setUniform("mProjection", camera->getProjection());
	shaderProgram->setUniform("mModel", mMat);
	shaderProgram->setUniform("viewPos", camera->getPosition());
	if (model.getMaterial().used)
	{
		shaderProgram->setUniform("material", model.getMaterial());
	}
	glBindBufferBase(GL_UNIFORM_BUFFER, lightingBlockId, lightingBuffer); //Bind lighting data
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model.getIndexSize()), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
#ifndef NDEBUG
	check = OpenGLSupport().GetError();
#endif
	glBindVertexArray(0);
}
