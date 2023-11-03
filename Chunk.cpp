#include "Chunk.h"

Chunk::Chunk(std::vector < std::pair<int, int>> blocks, glm::vec3 origin) {
	this->blocks = blocks;
	this->origin = origin;
}