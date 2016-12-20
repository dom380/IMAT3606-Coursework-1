#include "..\include\Model.h"

Model::Model(Graphics* graphics)
{
	this->graphics = graphics;
}

Model::~Model() {
	texture.reset();
}

void Model::init(char * objFile, char * textureFile)
{
	if (initalised) return;
	vector<glm::vec4> vertices; vector<glm::vec3> normals; vector<glm::vec2> textures; vector<unsigned short>indices;
	ObjReader().readObj(objFile, vertices, normals, textures, indices);
	
	graphics->bufferModelData(vertices, normals, textures, indices, vaoHandle);

	// Load the texture
	/*Bitmap bmp = Bitmap::bitmapFromFile(textureFile);
	bmp.flipVertically();
	texture = new Texture(bmp);*/
	texture = AssetManager::getInstance()->getTexture(textureFile);

	//indexSize = static_cast<GLsizei>(indices.size());
	indexSize = indices.size();

	shader.compileShader("./shaders/basic.vert", GL_VERTEX_SHADER);
	shader.compileShader("./shaders/basic.frag", GL_FRAGMENT_SHADER);
	shader.link();
	shader.bindShader();

	initalised = true;
}

void Model::render()
{
	if (!initalised) return;
	graphics->renderModel(*this, shader);
}

shared_ptr<Texture> Model::getTexture()
{
	return texture;
}

unsigned int Model::getVertArray()
{
	return vaoHandle;
}

size_t Model::getIndexSize()
{
	return indexSize;
}


