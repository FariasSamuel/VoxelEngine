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
#include "Block.h"
#include "ResourceManager.h"

#include <mutex>
#include <vector>

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
	unsigned int modelLoc;
	unsigned int viewLoc;
	unsigned int projectionLoc;
	std::vector<std::pair<int,int>> entities;
	std::vector<std::pair<int, int>> used;
	// constructor/destructor
	Game(unsigned int width, unsigned int height);
	~Game();
	// initialize game state (load all shaders/textures/levels)
	void Init();
	// game loop
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
	void generation(bool a);
	void destroier(GLFWwindow* window);
	bool perto(std::pair<int, int> i);
private:
	unsigned int VBO, VAO;
	bool procurar(int x, int z);
	int WORLD_SIZE = 40;
	
};
