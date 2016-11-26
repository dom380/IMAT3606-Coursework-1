#include "Shader.h"


Shader::Shader()
{
	linked = false;
	programHandle = 0;
}

Shader::~Shader()
{
	if (programHandle <= 0) {
		return;
	}
	//Get the number of and Ids of attached shaders
	GLint numShaders = 0;
	glGetProgramiv(programHandle, GL_ATTACHED_SHADERS, &numShaders);
	GLuint* shaderNames = new GLuint[numShaders];
	glGetAttachedShaders(programHandle, numShaders, NULL, shaderNames);
	for (int i = 0; i < numShaders; ++i) {
		glDeleteShader(shaderNames[i]);
	}
	glDeleteProgram(programHandle);
	delete[] shaderNames;
}

bool Shader::compileShader(const char * filePath, GLenum shaderType)
{
	if (programHandle <= 0) {
		programHandle = glCreateProgram();
	}
	ifstream inFile(filePath);
	if (!inFile) {
		fprintf(stderr, "Error opening file: %s\n", filePath);
		exit(1);
	}
	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	string codeStr(code.str());
	GLuint shaderHandle = glCreateShader(shaderType);

	const char * c_code = codeStr.c_str();
	glShaderSource(shaderHandle, 1, &c_code, NULL);

	// Compile the shader
	glCompileShader(shaderHandle);
	//Check result
	int result;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
	if (result == false) {
		fprintf(stderr, "Error compiling file: %s\n", filePath);
		GLint logLen;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shaderHandle, logLen, &written, log);
			fprintf(stderr, "Shader log: \n%s", log);
			free(log);
		}
		return false;
	}
	else {
		glAttachShader(programHandle, shaderHandle);
	}
	return true;
}

bool Shader::link()
{
	if (linked) {
		return true;
	}
	if (programHandle <= 0) {
		fprintf(stderr, "Error linking program. Shader program has not been compiled.\n");
		return false;
	}
	glLinkProgram(programHandle);
	int result;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &result);
	if (result == false) {
		fprintf(stderr, "Error linking program file.\n");
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen, &written, log);
			fprintf(stderr, "Program log: \n%s", log);
			free(log);
		}
		return false;
	}
	linked = true;
	return true;
}

void Shader::bindShader()
{
	if(linked && programHandle > 0)
		glUseProgram(programHandle);
}

void Shader::setUniform(const char * name, const vec2 & v)
{
	GLint location = glGetUniformLocation(programHandle, name);
	glUniform2f(location, v.x, v.y);
}

void Shader::setUniform(const char * name, const vec3 & v)
{
	GLint location = glGetUniformLocation(programHandle, name);
	glUniform3f(location, v.x, v.y, v.z);
}

void Shader::setUniform(const char * name, const vec4 & v)
{
	GLint location = glGetUniformLocation(programHandle, name);
	glUniform4f(location, v.x, v.y, v.z, v.w);
}

void Shader::setUniform(const char * name, const mat4 & m)
{
	GLint location = glGetUniformLocation(programHandle, name);
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(m));
}

void Shader::setUniform(const char * name, const mat3 & m)
{
	GLint location = glGetUniformLocation(programHandle, name);
	glUniformMatrix3fv(location, 1, false, glm::value_ptr(m));
}

void Shader::setUniform(const char * name, float val)
{
	GLint location = glGetUniformLocation(programHandle, name);
	glUniform1f(location, val);
}

void Shader::setUniform(const char * name, int val)
{
	GLint location = glGetUniformLocation(programHandle, name);
	glUniform1i(location, val);
}

void Shader::setUniform(const char * name, bool val)
{
	GLint location = glGetUniformLocation(programHandle, name);
	glUniform1i(location, val);
}

void Shader::setUniform(const char * name, GLuint val)
{
	GLint location = glGetUniformLocation(programHandle, name);
	glUniform1ui(location, val);
}

