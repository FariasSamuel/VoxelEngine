enum BlockType {
	BlockType_Default = 0,
	BlockType_Grass,
	BlockType_Dirt,
	BlockType_Water,
	BlockType_Stone,
	BlockType_Wood,
	BlockType_Sand,
	BlockType_NumTypes,
};

struct Block {
	bool m_active;
	BlockType m_blockType;
};