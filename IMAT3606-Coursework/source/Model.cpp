#include "..\include\Model.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Model::Model()
{
}

Model::~Model() {
	delete texture;
}

void Model::init(char * objFile, char * textureFile)
{
	if (initalised) return;
	vector<glm::vec4> vertices; vector<glm::vec3> normals; vector<glm::vec2> textures; vector<GLushort>indices;
	ObjReader().readObj(objFile, vertices, normals, textures, indices);
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
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textures.size(), glm::value_ptr(textures[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Creat VBO for normals
	GLuint normalBuffer = vboHandles[3];
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), glm::value_ptr(normals[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Create Vertex Array Object
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//Bind VBOs to VAO
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, (GLubyte *)NULL);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, (GLubyte *)NULL);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, (GLubyte *)NULL);
	//Bind IBO to VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
	
	//TODO Create AssetManager singleton rather than have indivual objects load and manage their stuff
	// Load the texture
	Bitmap bmp = Bitmap::bitmapFromFile(textureFile);
	bmp.flipVertically();
	texture = new Texture(bmp);

	//unbind VAO
	glBindVertexArray(0);
	
	indexSize = static_cast<GLsizei>(indices.size());
	initalised = true;
}

void Model::render(Shader &shaderProgram)
{
	//TODO Move to graphics object
	if (!initalised) return;
	shaderProgram.bindShader();
	glBindVertexArray(vaoHandle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->object());
	shaderProgram.setUniform("tex", 0);
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
	glBindVertexArray(0);
}


