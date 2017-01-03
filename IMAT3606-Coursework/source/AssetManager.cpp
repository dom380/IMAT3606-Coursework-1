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
	shared_ptr<Texture> ptr = std::make_shared<Texture>(bmp);
	textures.emplace(std::pair<string, shared_ptr<Texture>>(sFilePath, ptr));
	return ptr;	
}

shared_ptr<Font> AssetManager::getFont(char * fontPath, shared_ptr<Graphics>& graphics)
{
	string sFontPath = string(fontPath);
	auto it = fonts.find(sFontPath);
	if (it != fonts.end()) {
		return it->second;
	}
	FT_Library ft;
	FT_Error error = FT_Init_FreeType(&ft);
	shared_ptr<Font> fontPtr = std::make_shared<Font>(ft, fontPath, graphics);
	fontPtr->compile();
	fonts.emplace(std::pair<string, shared_ptr<Font>>(string(fontPath), fontPtr));
	return fontPtr;
}

shared_ptr<Shader> AssetManager::getShader(std::pair<string, string> shaderPath)
{
	auto it = shaders.find(shaderPath);
	if (it != shaders.end())
	{
		return it->second;
	}
	shared_ptr<Shader> shader = std::make_shared<Shader>();
	shader->compileShader(shaderPath.first.c_str(), GL_VERTEX_SHADER);
	shader->compileShader(shaderPath.second.c_str(), GL_FRAGMENT_SHADER);
	shader->link();
	shader->bindShader();
	shaders.emplace(std::pair<std::pair<string, string>, shared_ptr<Shader>>(shaderPath, shader));
	return shader;
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
