#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Vertex {
public:
	glm::vec3 positions, normals;
	glm::vec2 uvs;
	Vertex(glm::vec3 position, glm::vec2 uvs, glm::vec3 normals) {
		this->positions = position;
		this->normals = normals;
		this->uvs = uvs;
	}
};