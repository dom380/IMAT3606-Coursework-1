#pragma once
#ifndef OBJREADER_H
#define OBJREADER_H
#include <gl\glew.h>
#include <GL\glm\glm\glm.hpp>
#include <GL\glm\glm\gtx\hash.hpp>
#include <vector>
using std::vector;
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

class ObjReader {
public:
	ObjReader();
	~ObjReader();
	void readObj(char * filePath, vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<glm::vec2>& textures, vector<GLushort>&indices);
private:
	vector<glm::vec3> faceIndices; //x = position index. y = texture index. z = normal index
	std::unordered_map<glm::vec3, GLushort> indexVerticeMap;
	vector<glm::vec4> unpackedVertices;
	vector<glm::vec3> unpackedNormals;
	vector<glm::vec2> unpackedTextures;
};

#endif // !OBJREADER_H