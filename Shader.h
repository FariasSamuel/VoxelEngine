#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h> // include glad to get the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
public:
	// the program ID
	unsigned int ID;
	std::string data;
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void checkCompileErrors(unsigned int shader, std::string type);
};
#endif
