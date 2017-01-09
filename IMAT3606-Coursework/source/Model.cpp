#include "..\include\Model.h"

Model::Model(shared_ptr<Graphics>& graphics)
{
	this->graphics = graphics;
}

Model::~Model() {
	texture.reset();
}

void Model::init(const char * objFile, const char * textureFile, string id)
{
	if (initalised) return;
	shared_ptr<ModelData> modelData = AssetManager::getInstance()->getModelData(objFile, graphics);
	vboHandles = modelData->vboHandles;

	// Load the texture
	if(textureFile != NULL)
		texture = AssetManager::getInstance()->getTexture(textureFile);
	indexSize = modelData->indexSize;
	material = modelData->material;

	if (material.used)
	{
		shader = AssetManager::getInstance()->getShader(std::pair<string, string>("./shaders/phong.vert", "./shaders/phong.frag"));
	}
	else
	{
		shader = AssetManager::getInstance()->getShader(std::pair<string, string>("./shaders/basic.vert", "./shaders/basic.frag"));
	}
	this->id = id;
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
	if (vaoHandle != 0) return vaoHandle;
	vaoHandle = graphics->createVertexArrayObject(vboHandles);
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

string Model::getId()
{
	return id;
}

void Model::toggleDrawing()
{
	drawing = !drawing;
}

bool Model::isDrawing()
{
	return drawing;
}


