#pragma once
#include "Block.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
class Chunk {
public:
	std::vector < Block> blocks;
	glm::vec3 origin;
	Chunk();
	Chunk(std::vector<Block> blocks , glm::vec3 origin);
	//~Chunk();
	static const int CHUNK_SIZE = 16;
	
};
