#pragma once
#include "RawModel.h"
#include <glad/glad.h>
#include <vector>
class Loader {
public:
	 std::vector<unsigned int> vaos;
	 std::vector<unsigned int> vbos;

	RawModel LoadtoVAO(float vertices[]) {
		unsigned  vaoID = createVAO();
		storeDataAttribute(vertices, 0);
		glBindVertexArray(vaoID);
		RawModel rm(vaoID, sizeof(vertices) / sizeof(float));
		return rm;
	}
private:
	unsigned int createVAO() {
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		vaos.push_back(VAO);
		glBindVertexArray(VAO);
		
		return VAO;
	}

	void storeDataAttribute(float data[], int attributeNumber) {
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		vbos.push_back(VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		
	}

	void cleanUp() {
		for (int i = 0; vaos.size(); i++) {
			glDeleteBuffers(1, &vaos[i]);
		}
		for (int i = 0; vbos.size(); i++) {
			glDeleteBuffers(1, &vbos[i]);
		}
	}
};