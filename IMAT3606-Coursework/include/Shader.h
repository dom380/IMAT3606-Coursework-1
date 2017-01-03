#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <gl\glew.h>
#include <GL\glm\glm\glm.hpp>
#include <gl\glm\glm\gtc\type_ptr.hpp>
#include <Graphics\Light.h>
#include <Graphics\Material.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using glm::vec2; using glm::vec3; using glm::vec4; using glm::mat3; using glm::mat4;
using std::fstream; using std::ifstream; using std::string; using std::vector;


class Shader {
public:
	Shader();
	~Shader();
	bool compileShader(const char* filePath, GLenum shaderType);
	bool link();

	void bindShader();

	void   setUniform(const char *name, const vec2 & v);
	void   setUniform(const char *name, const vec3 & v);
	void   setUniform(const char *name, const vec4 & v);
	void   setUniform(const char *name, const mat4 & m);
	void   setUniform(const char *name, const mat3 & m);
	void   setUniform(const char *name, float val);
	void   setUniform(const char *name, int val);
	void   setUniform(const char *name, bool val);
	void   setUniform(const char *name, GLuint val);
	void   setUniform(const char *name, Light val);
	void   setUniform(const char *name, vector<Light> vals);
	void   setUniform(const char *name, Material val);

private:
	GLint programHandle = 0;
	bool linked;

};

#endif // !SHADER_H
