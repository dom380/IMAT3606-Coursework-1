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
class AssetManager {
public:
	static shared_ptr<AssetManager> getInstance();
	shared_ptr<Texture> getTexture(const char* filePath);
	shared_ptr<Font> getFont(char* fontPath, shared_ptr<Graphics>& graphics);
	void exit();
private:
	AssetManager() {};
	AssetManager(AssetManager const&) {}; // prevent copies
	void operator=(AssetManager const&) {}; // prevent assignments
	static bool initialised;
	static shared_ptr<AssetManager> instance;
	map<string, shared_ptr<Font>> fonts;
	map<string, shared_ptr<Texture>> textures;
};

#endif // !ASSETMANAGER_H
