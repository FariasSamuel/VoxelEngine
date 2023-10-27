
#include "Block.h"
class Chunk {
public:
	Chunk();
	~Chunk();
	void update(float dt);
	static const int CHUNK_SIZE = 16;
	void CreateMesh();
	void CreateCube();
private:
	Block*** m_pBlocks;
};
