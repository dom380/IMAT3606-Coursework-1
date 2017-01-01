#pragma once
#ifndef MODEL_H
#define MODEL_H
#include "Graphics\stb_image.h"
#include "Graphics\Bitmap.h"
#include "Graphics\Texture.h"
#include <Graphics\Camera.h>
#include "utils\ObjReader.h"
#include "Graphics.h"
#include "GL\glm\glm\glm.hpp"
#include "Shader.h"
#include "Graphics\Transform.h"
#include <vector>
using std::vector;
#include "AssetManager.h"

class Model {
public:
	Model(shared_ptr<Graphics>& graphics);
	~Model();
	void init(const char* objFile, const char* textureFile);
	void render(shared_ptr<Camera>& camera);
	shared_ptr<Texture> getTexture();
	unsigned int getVertArray();
	size_t getIndexSize();
	Transform transform;
private:
	bool initalised = false;
	shared_ptr<Graphics> graphics;
	unsigned int verticesId;
	unsigned int texCoordsId;
	unsigned int normalsId;
	unsigned int indexId;
	unsigned int textureId;

	size_t indexSize;

	unsigned int vboHandles[4];
	unsigned int vaoHandle;
	Shader shader;
	shared_ptr<Texture> texture;
};
#endif // !MODEL_H
