#pragma once
#ifndef FBXREADER_H
#define FBXREADER_H
#include "ModelFileReader.h"
/*
	Utility class to parse .fbx file types. 
	TODO
*/
class FbxReader : public ModelFileReader
{
	void readFile(const char * filePath, vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<glm::vec2>& textures, vector<unsigned short>&indices, Material& material) {};
};

#endif // !FBXREADER_H
