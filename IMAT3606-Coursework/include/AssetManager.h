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

class AssetManager {
public:
	static shared_ptr<AssetManager> getInstance();
	shared_ptr<Texture> getTexture(const char* filePath);
	shared_ptr<Font> getFont(char* fontPath, shared_ptr<Graphics>& graphics);
	shared_ptr<Shader> getShader(std::pair<string, string> shaderPath);
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
};

#endif // !ASSETMANAGER_H
