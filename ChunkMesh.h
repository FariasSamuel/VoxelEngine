#pragma once
#include <vector>
#include "Chunk.h"
#include "Vertex.h"
class ChunkMesh {
public:
	float *positions, *uvs, *normals;
	std::vector<Vertex> vertices;
	std::vector<float> positionsList;
	std::vector<float> uvsList;
	std::vector<float> normalsList;
	Chunk chunk;
	ChunkMesh();
	ChunkMesh(Chunk chunks);
	void update(Chunk chunk);
	void buildmesh();
	void populateLists();
	unsigned int VBO, VAO;
private:
	
};