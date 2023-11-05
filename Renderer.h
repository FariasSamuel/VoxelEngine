#pragma once
#include "RawModel.h"
#include <glad/glad.h>
class Renderer {
public:
	void render(RawModel rm) {
		glBindVertexArray(rm.vaoID);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, rm.vertexCount);
		glBindVertexArray(0);
	}
};