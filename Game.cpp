#include "game.h"
#include <vector>
#include "RawModel.h"
#include "Loader.h"
#include "Renderer.h"



Game::Game(unsigned int width, unsigned int height) : Keys(), Width(width), Height(height)
{
	this->Width = width;
	this->Height = height;
}

Game::~Game()
{

}

void Game::Init()
{
	s = ResourceManager::LoadShader("shader.vs", "shader.fs", nullptr, "test");
	s.Use();

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	unsigned int  VBO2;

	glGenVertexArrays(1, &vaoCube);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(vaoCube);

	glBindBuffer(GL_ARRAY_BUFFER, vaoCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	//std::cout << sizeof(float) * entities[index].positionsList.size() << std::endl;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	projection = pespective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 100.0f);

	this->modelLoc = glGetUniformLocation(s.ID, "model");
	this->viewLoc = glGetUniformLocation(s.ID, "view");
	this->projectionLoc = glGetUniformLocation(s.ID, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glEnable(GL_DEPTH_TEST);
}

void Game::generation(bool a) {
	while (true) {
		//std::cout << "hello" << std::endl;

		std::lock_guard<std::mutex> guard(g_pages_mutex);
		try {
			for (int i = (int)(this->camera.Position.x - 64) / 32; i < (int)(this->camera.Position.x + 64) / 32; i++) {
				for (int j = (int)(this->camera.Position.z - 64) / 32; j < (int)(this->camera.Position.z + 64) / 32; j++) {
					if (std::find(used.begin(), used.end(), std::make_pair(i* 32, j* 32)) == used.end()) {
						std::vector<Block> blocks;

						for (int x = 0; x < 32; x++) {
							for (int y = 0; y < 32; y++) {
								int h = pl.generateHeight(x + (i* 32), y+(j* 32));
								Block b(x, h ,y, orange);
								blocks.push_back(b);
							}
						}
						Chunk c(blocks, glm::vec3(i * 32, 0 , j * 32));
						ChunkMesh m(c);
						
						this->used.push_back(std::make_pair(i * 32, j * 32));
						
						entities.push_back(m);
					}
				}
			}
		}
		catch (const std::exception& oor) {
			std::cerr << "Out of Range error: " << oor.what() << '\n';
		}


	}
}

void Game::Update(float dt)
{

}
bool Game::procurar(Block b) {
	for (auto i : blocos) {
		if (i.x == b.x && i.y == b.y && i.z == b.z) {
			return true;
		}
	}
	return false;
}
void Game::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		int x = (int)camera.Position.x + floor((int)(5 + (camera.Pitch + 33) / 11) * sin(glm::radians(90 - camera.Yaw)));
		int y = (int)camera.Position.y - 2;
		int z = (int)camera.Position.z + floor((int)(5 + (camera.Pitch + 33) / 11) * cos(glm::radians(90 - camera.Yaw)));
		Block c(x, y, z,orange);
		if (!procurar(c)) {
			blocos.push_back(c);
			
		}
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		int x = (int)camera.Position.x + floor((int)(5 + (camera.Pitch + 33) / 11) * sin(glm::radians(90 - camera.Yaw)));
		int y = (int)camera.Position.y - 2;
		int z = (int)camera.Position.z + floor((int)(5 + (camera.Pitch + 33) / 11) * cos(glm::radians(90 - camera.Yaw)));
		Block c(x, y, z,orange);
		auto v = std::remove_if(blocos.begin(), blocos.end(), [c](Block i) { return  (i.x == c.x) && (i.y == c.y) && (i.z == c.z);  });
		std::cout << "BLoco:" << c.x << " " << c.y << " " << c.z << std::endl;
		for (int i = 0; i < entities.size(); i++)
		{
			glm::vec3 origin = entities[i].chunk.origin;

			int disx = (int)abs(origin.x - camera.Position.x);
			int disz = (int)abs(origin.z - camera.Position.z);

			if (disx <= 32 && disz <= 32) {
				Chunk chunk = entities[i].chunk;
				//std::cout << chunk.origin.x << " " << chunk.origin.y << " " << chunk.origin.z << std::endl;
				auto v1 = std::remove_if(chunk.blocks.begin(), chunk.blocks.end(), [c,chunk](Block i) { return  (c.x == (i.x + chunk.origin.x)) && (i.y == c.y) && (c.z == (i.z + chunk.origin.z));  });
				if (v1 != chunk.blocks.end()) {
					flag = i;
					entities[i].update(chunk);
					std::cout << "encontrei";
					break;
				}
				std::cout << std::endl;
				for (auto j : entities[i].chunk.blocks) {
					
					//if (j.x == c.x && j.y == c.y) {
						//std::cout << j.x << " " << j.y << " " << j.z << std::endl;
					//}
				}

			}

		}

	}
}

void Game::Render()
{

	s.Use();
	view = camera.getView();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	s.Use();
	if (index < entities.size() && entities.size() > 0) {
		unsigned int VAO2, VBO2;

		glGenVertexArrays(1, &VAO2);
		glGenBuffers(1, &VBO2);
		glBindVertexArray(VAO2);

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * entities[index].positionsList.size(), &entities[index].positionsList[0], GL_STATIC_DRAW);
		//std::cout << sizeof(float) * entities[index].positionsList.size() << std::endl;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		//glBindVertexArray(0);
		VAO.push_back(VAO2);
		VBO.push_back(VBO2);
		index++;
	}
	if (flag != -1) {
		unsigned int VAO2, VBO2;
		std::cout << "mudando" << std::endl;
		glGenVertexArrays(1, &VAO2);
		glGenBuffers(1, &VBO2);
		glBindVertexArray(VAO2);

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * entities[flag].positionsList.size(), &entities[flag].positionsList[0], GL_STATIC_DRAW);
		//std::cout << sizeof(float) * entities[index].positionsList.size() << std::endl;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		VAO[flag] = VAO2;
		VBO[flag] = VBO2;
		flag = -1;
	}
	try {
		//std::cout << entities.size() << std::endl;

		for (int i = 0; i < VAO.size(); i++)
		{			
			glm::vec3 origin = entities[i].chunk.origin;

			int disx = (int)abs(origin.x - camera.Position.x);
			int disz = (int)abs(origin.z - camera.Position.z);

			if (disx <=  32 && disz <= 32) {

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(origin.x, origin.y, origin.z));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

				glBindVertexArray(VAO[i]);
				glDrawArrays(GL_TRIANGLES, 0, entities[i].positionsList.size());
				//glBindVertexArray(0);
			}

		}
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((int)camera.Position.x + floor((int)(5 + (camera.Pitch + 33) / 11) * sin(glm::radians(90 - camera.Yaw))), (int)camera.Position.y-2, (int)camera.Position.z + floor((int)(5 + (camera.Pitch + 33) / 11) * cos(glm::radians(90 - camera.Yaw)))));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(vaoCube);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		for (int i = 0; i < blocos.size(); i++)
		{

			int disx = (int)abs(blocos[i].x - camera.Position.x);
			int disz = (int)abs(blocos[i].z - camera.Position.z);

			if (disx <= 32 && disz <= 32) {

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(blocos[i].x, blocos[i].y, blocos[i].z));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

				glBindVertexArray(vaoCube);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				//glBindVertexArray(0);
			}

		}
	}
	catch (const std::exception& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}

	s.Use();
}

void Game::cleanUp() {
	for (int i = 0; VAO.size(); i++) {
		glDeleteBuffers(1, &VAO[i]);
	}
	for (int i = 0; VBO.size(); i++) {
		glDeleteBuffers(1, &VBO[i]);
	}
}