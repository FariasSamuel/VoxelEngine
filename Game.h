#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Operations.h"
#include "Shader.h"
#include "Camera.h"
#include "ResourceManager.h"
#include <mutex>
#include <vector>
#include "Chunk.h"
#include "CubeModel.h"
#include "ChunkMesh.h"
#include "Renderer.h"
#include "PerlinNoiseGenerator.h"

class Game
{
public:
	bool Keys[1024];
	std::mutex g_pages_mutex;
	unsigned int Width, Height;
	Camera camera;
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	Shader s;
	unsigned int vaoCube;
	unsigned int modelLoc;
	unsigned int viewLoc;
	unsigned int projectionLoc;
	int size;
	PerlinNoiseGenerator pl;
	std::vector<ChunkMesh> entities;
	std::vector<Block> blocos;
	std::vector<std::pair<int, int>> used;
	
	Game(unsigned int width, unsigned int height);
	~Game();
	void Init();
	void ProcessInput(GLFWwindow* window);
	void Update(float dt);
	void Render();
	void generation(bool a);
	bool procurar(Block b);
	void cleanUp();
	int index = 0;
	int flag = -1;
private:
	std::vector<unsigned int> VBO, VAO;
	int WORLD_SIZE = 16;
	
};
