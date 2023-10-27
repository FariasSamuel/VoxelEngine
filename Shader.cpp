#include <iostream>
#include <fstream>
#include <string>
#include "Shader.h"

using namespace std;

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource) {
		

		// 2. compile shaders
	unsigned int vertex, fragment, geometry;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		
		if (geometrySource != nullptr)
		{
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &geometrySource, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
		}

		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertex);
		glAttachShader(this->ID, fragment);
		if (geometrySource != nullptr)
			glAttachShader(this->ID, geometry);
		glLinkProgram(this->ID);
		checkCompileErrors(this->ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (geometrySource != nullptr)
			glDeleteShader(geometry);
}

Shader &Shader:: Use() {
	glUseProgram(this->ID);
	return *this;
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
		else {
			std::cout << "oi" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
		else {
			std::cout << "oi" << std::endl;
		}
	}
}