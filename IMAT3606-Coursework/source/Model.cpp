#include "..\include\Model.h"

Model::Model(shared_ptr<Graphics>& graphics)
{
	this->graphics = graphics;
}

Model::~Model() {
	texture.reset();
}

void Model::init(const char * objFile, const char * textureFile)
{
	if (initalised) return;
	vector<glm::vec4> vertices; vector<glm::vec3> normals; vector<glm::vec2> textures; vector<unsigned short>indices;
	ObjReader().readObj(objFile, vertices, normals, textures, indices, material);
	
	graphics->bufferModelData(vertices, normals, textures, indices, vaoHandle);

	// Load the texture
	texture = AssetManager::getInstance()->getTexture(textureFile);

	indexSize = indices.size();

	if (material.used)
	{
		shader = AssetManager::getInstance()->getShader(std::pair<string, string>("./shaders/phong.vert", "./shaders/phong.frag"));
		//shader.compileShader("./shaders/phong.vert", GL_VERTEX_SHADER);
		//shader.compileShader("./shaders/phong.frag", GL_FRAGMENT_SHADER);
	}
	else
	{
		shader = AssetManager::getInstance()->getShader(std::pair<string, string>("./shaders/basic.vert", "./shaders/basic.frag"));
		//shader.compileShader("./shaders/basic.vert", GL_VERTEX_SHADER);
		//shader.compileShader("./shaders/basic.frag", GL_FRAGMENT_SHADER);
	}
	//shader.link();
	//shader.bindShader();
	
	initalised = true;
}

void Model::render(shared_ptr<Camera>& camera)
{
	if (!initalised) return;
	graphics->renderModel(*this, shader, camera);
}

void Model::render(shared_ptr<Camera>& camera, vector<Light> lights)
{
	if (!initalised) return;
	graphics->renderModel(*this, shader, camera, lights);
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

Material Model::getMaterial()
{
	return material;
}


