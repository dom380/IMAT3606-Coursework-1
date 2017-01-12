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
	shared_ptr<Texture> getTexture(const char* fileName);
	shared_ptr<Font> getFont(char* fontPath, shared_ptr<Graphics>& graphics);
	shared_ptr<Shader> getShader(std::pair<string, string> shaderName);
	shared_ptr<ModelData> getModelData(const char* fileName, shared_ptr<Graphics> graphics);
	enum ResourceType {
		LEVEL,
		TEXTURE,
		MODEL,
		FONT,
		SHADER
	};
	string getRootFolder(ResourceType resourceType);
	void setAssetFolder(string path, ResourceType resourceType);
	void exit();
private:
	AssetManager() {};
	AssetManager(AssetManager const&) {}; // prevent copies
	void operator=(AssetManager const&) {}; // prevent assignments
	string buildFilePath(ResourceType resourceType, const char* path);
	static bool initialised;
	static shared_ptr<AssetManager> instance;
	map<string, shared_ptr<Font>> fonts;
	map<string, shared_ptr<Texture>> textures;
	map<std::pair<string, string>, shared_ptr<Shader>> shaders;
	map<string, shared_ptr<ModelData>> modelData;
	string fontFolder;
	string levelFolder;
	string modelFolder;
	string textureFolder;
	string shaderFolder;
};

#endif // !ASSETMANAGER_H
