#pragma once
#ifndef MODEL_H
#define MODEL_H
#include "utils\stb_image.h"
#include "utils\Bitmap.h"
#include "utils\Texture.h"
#include "utils\ObjReader.h"
#include "Graphics.h"
#include "GL\glm\glm\glm.hpp"
#include "Shader.h"
#include "utils\Transform.h"
#include <vector>
using std::vector;
#include "AssetManager.h"

class Model {
public:
	Model(Graphics* graphics);
	~Model();
	void init(char* objFile, char* textureFile);
	void render();
	shared_ptr<Texture> getTexture();
	unsigned int getVertArray();
	size_t getIndexSize();
	Transform transform;
private:
	bool initalised = false;
	Graphics* graphics;
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
