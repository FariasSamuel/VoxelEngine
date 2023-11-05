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
	std::vector<Block> blocks;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			BlockType bt = BlockType::orange;
			Block b(x , 0, y, bt);
			blocks.push_back(b);
		}
	}

	Chunk c(blocks, glm::vec3(10, 0, 0));
	ChunkMesh m(c);
	//entities.push_back(m);
	/*unsigned int VAO1, VBO1;
	
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glBindVertexArray(VAO1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.positionsList.size(), &m.positionsList[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	this->VAO.push_back(VAO1);
	this->VBO.push_back(VBO1);
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			for (int z = 0; z < 8; z++) {
				BlockType bt = BlockType::orange;
				Block b(x, y, y-z, bt);
				blocks.push_back(b);
				
			}
		}
	}
	Chunk c1(blocks, glm::vec3(15, 0, 0));
	ChunkMesh m1(c1);
	entities.push_back(m1);
	unsigned int VAO2, VBO2;

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m1.positionsList.size(), &m1.positionsList[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	this->VAO.push_back(VAO2);
	this->VBO.push_back(VBO2);
	*/
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

void Game::ProcessInput(float dt)
{

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