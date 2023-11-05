#include "Chunk.h"

Chunk::Chunk(std::vector < Block> blocks, glm::vec3 origin) {
	this->blocks = blocks;
	this->origin = origin;
}
Chunk::Chunk() {
	std::vector<Block> b;
	this->blocks =b;
	this->origin = glm::vec3(0.0f,0.0f,0.0f);
}