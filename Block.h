#pragma once
enum BlockType
{
	orange = 0,
	green = 1,
	blue = 1,
};

class Block {
public: 
	int x, y, z;
	 BlockType type;
	Block(int x, int y, int z, BlockType t) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->type = t;
	}
};