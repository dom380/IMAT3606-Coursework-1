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
#include <Graphics\Shader.h>
#include <Graphics\ModelData.h>
#include <utils\ObjReader.h>
/*
	Singleton class responsible for loading and storing all Assets.
*/
class AssetManager {
public:
	/*
		Returns the AssetManager instance or creates one if it doesn't exist.
	*/
	static shared_ptr<AssetManager> getInstance();
	/*
		Load and/or Return a Pointer to a Texture
		const char* fileName, The name of the texture to retrieve.
	*/
	shared_ptr<Texture> getTexture(const char* fileName);
	/*
		Load and/or Return a Pointer to a Font.
		const char* fileName, The name of the Font to retrieve.
		shared_ptr<Graphics>& graphics, Pointer to the graphics system.
	*/
	shared_ptr<Font> getFont(char* fontPath, shared_ptr<Graphics>& graphics);
	/*
		Load and/or Return a Pointer to a Font.
		std::pair<string, string> shaderName, A std::pair of strings specifying the vertex and fragment shaders.
	*/
	shared_ptr<Shader> getShader(std::pair<string, string> shaderName);
	/*
		Load and/or Return a Pointer to ModelData.
		const char* fileName, The name of the model to retrieve.
		shared_ptr<Graphics>& graphics, Pointer to the graphics system.
	*/
	shared_ptr<ModelData> getModelData(const char* fileName, shared_ptr<Graphics> graphics);
	/*
		Enum specifying the type of resource.
	*/
	enum ResourceType {
		LEVEL,
		TEXTURE,
		MODEL,
		FONT,
		SHADER
	};
	/*
		Returns the file path of to the folder containing files of the specified resource type.
	*/
	string getRootFolder(ResourceType resourceType);
	/*
		Sets the Root folder of the specified resource type.
		string path, File path to the folder containing the resource. Must include a trailing '/'.
		ResourceType resourceType, Enum specifying the type of resource.
	*/
	void setAssetFolder(string path, ResourceType resourceType);
	/*
		Clean up resources.
	*/
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
