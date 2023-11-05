#include "ChunkMesh.h"
#include "CubeModel.h"
#include "Vertex.h"
#include <iostream>
#include <glad/glad.h>

ChunkMesh::ChunkMesh() {
	std::cout << "tchau";
}


ChunkMesh::ChunkMesh(Chunk chunks) {
	this->chunk = chunks;
	std::cout << "oi";
	buildmesh();
	populateLists();

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->positionsList.size(), &this->positionsList[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void ChunkMesh::update(Chunk chunk) {
	this->chunk = chunk;
	buildmesh();
	populateLists();
}

void ChunkMesh::buildmesh() {
	for (int i = 0; i < chunk.blocks.size(); i++) {
		Block blockI = chunk.blocks[i];
		//std::cout << blockI.x << " " << blockI.y << " " << blockI.z << std::endl;
		bool px = false, nx = false, py = false, ny = false, pz = false, nz = false;
		for (int j = 0; j < chunk.blocks.size(); j++) {
			Block blockJ = chunk.blocks[j];
			
			//std::cout << blockJ.x << " " << blockJ.y << " " << blockJ.z << std::endl;
			
			if (((blockI.x + 1) == (blockJ.x)) && ((blockI.y) == (blockJ.y)) && ((blockI.z) == (blockJ.z))) {
				px = true;
				//std::cout << "px " << i <<" " << j << std::endl;
			}
			if (((blockI.x - 1) == (blockJ.x)) && ((blockI.y) == (blockJ.y)) && ((blockI.z) == (blockJ.z))) {
				nx = true;
				//std::cout << "nx " << i << " " << j << std::endl;
			}
			if (((blockI.x) == (blockJ.x)) && ((blockI.y + 1) == (blockJ.y)) && ((blockI.z) == (blockJ.z))) {
				py = true;
				//std::cout << "py " << i << " " << j << std::endl;
			}
			if (((blockI.x) == (blockJ.x)) && ((blockI.y - 1) == (blockJ.y)) && ((blockI.z) == (blockJ.z))) {
				ny = true;
				//std::cout << "ny " << i << " " << j << std::endl;
			}
			if (((blockI.x) == (blockJ.x)) && ((blockI.y) == (blockJ.y)) && ((blockI.z + 1) == (blockJ.z))) {
				pz = true;
				//std::cout << "pz " << i << " " << j << std::endl;
			}
			if (((blockI.x) == (blockJ.x)) && ((blockI.y) == (blockJ.y)) && ((blockI.z - 1) == (blockJ.z))) {
				nz = true;
				//std::cout << "nz " << i << " " << j << std::endl;
			}
		}
			if (!px) {
				for (int k = 0; k < 6; k++) {
					Vertex v(glm::vec3(CubeModel::PX_POS[k].x + blockI.x, CubeModel::PX_POS[k].y + blockI.y, CubeModel::PX_POS[k].z + blockI.z), CubeModel::UV[k], CubeModel::NORMALS[k]);
					vertices.push_back(v);
				}
			}
			if (!nx) {
				for (int k = 0; k < 6; k++) {
					Vertex v(glm::vec3(CubeModel::NX_POS[k].x + blockI.x, CubeModel::NX_POS[k].y + blockI.y, CubeModel::NX_POS[k].z + blockI.z), CubeModel::UV[k], CubeModel::NORMALS[k]);
					vertices.push_back(v);
				}
			}
			if (!py) {
				for (int k = 0; k < 6; k++) {
					Vertex v(glm::vec3(CubeModel::PY_POS[k].x + blockI.x, CubeModel::PY_POS[k].y + blockI.y, CubeModel::PY_POS[k].z + blockI.z), CubeModel::UV[k], CubeModel::NORMALS[k]);
					vertices.push_back(v);
				}
			}
			if (!ny) {
				for (int k = 0; k < 6; k++) {
					Vertex v(glm::vec3(CubeModel::NY_POS[k].x + blockI.x, CubeModel::NY_POS[k].y + blockI.y, CubeModel::NY_POS[k].z + blockI.z), CubeModel::UV[k], CubeModel::NORMALS[k]);
					vertices.push_back(v);
				}
			}
			if (!pz) {
				for (int k = 0; k < 6; k++) {
					Vertex v(glm::vec3(CubeModel::PZ_POS[k].x + blockI.x, CubeModel::PZ_POS[k].y + blockI.y, CubeModel::PZ_POS[k].z + blockI.z), CubeModel::UV[k], CubeModel::NORMALS[k]);
					vertices.push_back(v);
				}
			}
			if (!nz) {
				for (int k = 0; k < 6; k++) {
					Vertex v(glm::vec3(CubeModel::NZ_POS[k].x + blockI.x, CubeModel::NZ_POS[k].y + blockI.y, CubeModel::NZ_POS[k].z + blockI.z), CubeModel::UV[k], CubeModel::NORMALS[k]);
					vertices.push_back(v);
				}
			}
		
	}
}

void ChunkMesh::populateLists() {
	for (int i = 0; i < vertices.size(); i++) {
		positionsList.push_back(vertices[i].positions.x);
		positionsList.push_back(vertices[i].positions.y);
		positionsList.push_back(vertices[i].positions.z);
		uvsList.push_back(vertices[i].uvs.x);
		uvsList.push_back(vertices[i].uvs.y);
		normalsList.push_back(vertices[i].normals.x);
		normalsList.push_back(vertices[i].normals.y);
		normalsList.push_back(vertices[i].normals.z);
	}
	//std::cout << "size:" << positionsList.size() << std::endl;
	positions = &positionsList[0];
	//uvsList = new float[uvsList.size()];
	normals = new float[normalsList.size()]();
	/*for (int i = 0; i < normalsList.size() / 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << positions[i+j] << " ";
		}
		std::cout << std::endl;
	}*/
	for (int i = 0; i < normalsList.size(); i++) {
		normals[i] = normalsList[i];
	}
}