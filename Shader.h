#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h> // include glad to get the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
public:
	unsigned int ID;
	Shader() {};
	Shader& Use();															
	void Compile(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
    void    SetFloat(const char* name, float value, bool useShader = false);
    void    SetInteger(const char* name, int value, bool useShader = false);
    void    SetVector2f(const char* name, float x, float y, bool useShader = false);
    void    SetVector2f(const char* name, const glm::vec2& value, bool useShader = false);
    void    SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
    void    SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
    void    SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
    void    SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
    void    SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};
#endif
