
#include "Block.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
class Chunk {
public:
	std::vector < std::pair<int, int>> blocks;
	glm::vec3 origin;
	Chunk(std::vector<std::pair<int,int>> blocks, glm::vec3 origin);
	//~Chunk();
	static const int CHUNK_SIZE = 16;
	
};
