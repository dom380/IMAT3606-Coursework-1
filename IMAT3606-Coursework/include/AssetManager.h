#pragma once
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H
#include <memory>
using std::shared_ptr;
#include <map>
using std::map;
#include <Graphics\Texture.h>
#include <string>
using std::string;
#include <GUI\Font.h>
#include <Shader.h>
#include <Graphics\ModelData.h>
#include <utils\ObjReader.h>

class AssetManager {
public:
	static shared_ptr<AssetManager> getInstance();
	shared_ptr<Texture> getTexture(const char* filePath);
	shared_ptr<Font> getFont(char* fontPath, shared_ptr<Graphics>& graphics);
	shared_ptr<Shader> getShader(std::pair<string, string> shaderPath);
	shared_ptr<ModelData> getModelData(const char* filePath, shared_ptr<Graphics> graphics);
	void exit();
private:
	AssetManager() {};
	AssetManager(AssetManager const&) {}; // prevent copies
	void operator=(AssetManager const&) {}; // prevent assignments
	static bool initialised;
	static shared_ptr<AssetManager> instance;
	map<string, shared_ptr<Font>> fonts;
	map<string, shared_ptr<Texture>> textures;
	map<std::pair<string, string>, shared_ptr<Shader>> shaders;
	map<string, shared_ptr<ModelData>> modelData;
};

#endif // !ASSETMANAGER_H
