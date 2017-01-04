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
	}
	else
	{
		shader = AssetManager::getInstance()->getShader(std::pair<string, string>("./shaders/basic.vert", "./shaders/basic.frag"));
	}
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

void Model::render(shared_ptr<Camera>& camera, unsigned int lightingBuffer, unsigned int lightingBlockId)
{
	if (!initalised) return;
	graphics->renderModel(*this, shader, camera, lightingBuffer, lightingBlockId);
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


