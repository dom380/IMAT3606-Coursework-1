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

shared_ptr<Font> AssetManager::getFont(char * fontPath, Graphics* graphics)
{
	string sFontPath = string(fontPath);
	auto it = fonts.find(sFontPath);
	if (it != fonts.end()) {
		return it->second;
	}
	FT_Library ft;
	FT_Error error = FT_Init_FreeType(&ft);
	shared_ptr<Font> fontPtr(new Font(ft, fontPath, graphics));
	fontPtr->compile();
	fonts.emplace(std::pair<string, shared_ptr<Font>>(string(fontPath), fontPtr));
	return fontPtr;
}

void AssetManager::exit()
{
	//probably don't need this but be extra sure to free memory
	for (auto it : textures) {
		it.second.reset();
	}
	for (auto it : fonts) {
		it.second.reset();
	}
	textures.clear();
	fonts.clear();
}
