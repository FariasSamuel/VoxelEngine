#pragma once

class RawModel {
public:
	int vaoID;
	int vertexCount;

	RawModel(int vaoID, int vertexCount) {
		this->vaoID = vaoID;
		this->vertexCount = vertexCount;
	}
};