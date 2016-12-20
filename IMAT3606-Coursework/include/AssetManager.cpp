#include "AssetManager.h"

bool AssetManager::initialised = false;
shared_ptr<AssetManager> AssetManager::instance;

shared_ptr<AssetManager> AssetManager::getInstance()
{
	if (initialised) {
		return instance;
	}
	instance = shared_ptr<AssetManager>(new AssetManager());
	initialised = true;
	return instance;
}

shared_ptr<Texture> AssetManager::getTexture(const char * filePath)
{
	string sFilePath = string(filePath);
	auto it = textures.find(sFilePath);
	if (it != textures.end()) {
		return it->second;
	}
	Bitmap bmp = Bitmap::bitmapFromFile(filePath);
	bmp.flipVertically();
	shared_ptr<Texture> ptr(new Texture(bmp));
	textures.emplace(std::pair<string, shared_ptr<Texture>>(sFilePath, ptr));
	return ptr;	
}

void AssetManager::exit()
{
	//probably don't need this but be extra sure to free memory
	for (auto it : textures) {
		it.second.reset();
	}
	textures.clear();
}
