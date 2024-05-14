#include "game.h"
#include <vector>
#include "RawModel.h"
#include "Loader.h"
#include "Renderer.h"
#include "ray.h"


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
	lasttime = glfwGetTime();
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

vec3 intersect(const ray &r, Block bloco) {
	vec3 b(bloco.x, bloco.y, bloco.z);
	float tmin = (b.x() - 0.5 - r.origin().x()) / r.direction().x();
	float tmax = (b.x() + 0.5 - r.origin().x()) / r.direction().x();

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (b.y() - 0.5 - r.origin().y()) / r.direction().y();
	float tymax = (b.y() + 0.5 - r.origin().y()) / r.direction().y();

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return vec3(0,0,0);

	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	float tzmin = (b.z() - 0.5 - r.origin().z()) / r.direction().z();
	float tzmax = (b.z() + 0.5 - r.origin().z()) / r.direction().z();

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return vec3(0, 0, 0);

	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	return vec3(tmin, tymin, tzmin);

}

vec3 intersect(const ray& r, vec3 bloco) {
	vec3 b(bloco.x(), bloco.y(), bloco.z());
	float tmin = (b.x() - 0.5 - r.origin().x()) / r.direction().x();
	float tmax = (b.x() + 0.5 - r.origin().x()) / r.direction().x();

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (b.y() - 0.5 - r.origin().y()) / r.direction().y();
	float tymax = (b.y() + 0.5 - r.origin().y()) / r.direction().y();

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return vec3(0, 0, 0);

	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	float tzmin = (b.z() - 0.5 - r.origin().z()) / r.direction().z();
	float tzmax = (b.z() + 0.5 - r.origin().z()) / r.direction().z();

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return vec3(0, 0, 0);

	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	return vec3(tmin, tymin, tzmin);

}

void Game::ProcessInput(GLFWwindow* window)
{
	if (glfwGetTime() - lasttime  < 0.1)
		return;
	lasttime = (float)glfwGetTime();
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		vec3 positionCam(camera.Position.x, camera.Position.y, camera.Position.z);

		for (int i = 0; i < entities.size(); i++)
		{
			glm::vec3 origin = entities[i].chunk.origin;

			int disx = (int)abs(origin.x - camera.Position.x);
			int disz = (int)abs(origin.z - camera.Position.z);

			if (disx <= 32 && disz <= 32) {
				Chunk chunk = entities[i].chunk;
				vec3 h;
				int indice = -1;
				float dist = 100;
				int q = 0;
				ray vision = ray(point3(camera.Position.x, camera.Position.y, camera.Position.z), vec3(camera.Front.x, camera.Front.y, camera.Front.z));
				for (auto b : chunk.blocks) {
					//std::cout << vec3(b.x + chunk.origin.x, b.y , b.z + chunk.origin.z) << " " <<  point3(c.x, c.y, c.z) << " " << vec3(camera.Front.x, camera.Front.y, camera.Front.z) << std::endl;
					vec3 hit = intersect(vision, vec3(b.x + chunk.origin.x, b.y, b.z + chunk.origin.z));
					if (vec3(0, 0, 0) != hit) {
						float d = (vision.at(hit.x()) - positionCam).length_squared();
						if (dist > d) {
							dist = d;
							indice = q;
							h = hit;
						}
					}
					q++;
				}

				if (indice != -1) {
					//std::cout << vision.at(h.x()) << " " << vec3(float(chunk.blocks[indice].x + chunk.origin.x), chunk.blocks[indice].y , chunk.blocks[indice].z + chunk.origin.z) << std::endl;
					
					/*float dpy = vision.at(h.x()).y() - (chunk.blocks[indice].y - 0.5);
					float dpx = vision.at(h.x()).x() - (chunk.blocks[indice].x + 0.5);
					float dpz = vision.at(h.x()).z() - (chunk.blocks[indice].z + 0.5);
					float dnx = vision.at(h.x()).x() - (chunk.blocks[indice].x - 0.5);
					float dnz = vision.at(h.x()).z() - (chunk.blocks[indice].z - 0.5);
					float m = std::min(dpy,std::min(dpx,std::min(dpz,std::min(dnx,dnz))));
					*/
					//std::cout << dpy << " " << dpx << " " << dpz << " " << dnx << " " << dnz << " " << std::endl;
					double vx = vision.at(h.x()).x();
					double vy = vision.at(h.x()).y();
					double vz = vision.at(h.x()).z();
					double bx = chunk.blocks[indice].x + chunk.origin.x;
					double by = chunk.blocks[indice].y;
					double bz = chunk.blocks[indice].z + chunk.origin.z;
					std::cout << vx << " " << vy << " " << vz << " " << bx << " " << by << " " << " " << bz << std::endl;
					std::cout << abs(vy - (by - 0.5)) << " " << abs(vx - (bx + 0.5)) << " " << abs(vx - (bx - 0.5)) << " " << abs(vz - (bz + 0.5)) << " " << abs(vz - (bz - 0.5)) << std::endl;
					if (abs(vy - (by - 0.5)) < 0.0001) {
						Block c(chunk.blocks[indice].x, chunk.blocks[indice].y + 1, chunk.blocks[indice].z , orange);
						//Block c(h.x()+1, h.y(), h.z()+1, orange);]
						std::cout << "posto 1" << std::endl;
						entities[i].chunk.blocks.push_back(c);
						flag = i;
						entities[i].update(entities[i].chunk);
						break;
					}
					if (abs(vy - (by + 0.5)) < 0.0001) {
						Block c(chunk.blocks[indice].x, chunk.blocks[indice].y + 1, chunk.blocks[indice].z, orange);
						//Block c(h.x()+1, h.y(), h.z()+1, orange);]
						std::cout << "posto 1" << std::endl;
						entities[i].chunk.blocks.push_back(c);
						flag = i;
						entities[i].update(entities[i].chunk);
						break;
					}
					 if (abs(vx - (bx + 0.5))  < 0.0001) {
							Block c(chunk.blocks[indice].x + 1, chunk.blocks[indice].y , chunk.blocks[indice].z , orange);
							//Block c(h.x()+1, h.y(), h.z()+1, orange);]
							std::cout << "posto 2" << std::endl;
							entities[i].chunk.blocks.push_back(c);
							flag = i;
							entities[i].update(entities[i].chunk);
							break;
					}
					 if (abs(vx - (bx - 0.5)) < 0.0001) {
						Block c(chunk.blocks[indice].x - 1, chunk.blocks[indice].y, chunk.blocks[indice].z, orange);
						//Block c(h.x()+1, h.y(), h.z()+1, orange);]
						std::cout << "posto 3" << std::endl;
						entities[i].chunk.blocks.push_back(c);
						flag = i;
						entities[i].update(entities[i].chunk);
						break;
					}
					 if (abs(vz - (bz + 0.5)) < 0.0001) {
						Block c(chunk.blocks[indice].x , chunk.blocks[indice].y, chunk.blocks[indice].z+1, orange);
						//Block c(h.x()+1, h.y(), h.z()+1, orange);]
						std::cout << "posto 4" << std::endl;
						entities[i].chunk.blocks.push_back(c);
						flag = i;
						entities[i].update(entities[i].chunk);
						break;
					}
					if (abs(vz - (bz - 0.5)) < 0.0001) {
						Block c(chunk.blocks[indice].x , chunk.blocks[indice].y, chunk.blocks[indice].z -1, orange);
						//Block c(h.x()+1, h.y(), h.z()+1, orange);]
						std::cout << "posto 5" << std::endl;
						entities[i].chunk.blocks.push_back(c);
						flag = i;
						entities[i].update(entities[i].chunk);
						break;
					}
				}
			
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		int x = (int)camera.Position.x + floor((int)(5 + (camera.Pitch + 33) / 11) * sin(glm::radians(90 - camera.Yaw)));
		int y = (int)camera.Position.y - 2;
		int z = (int)camera.Position.z + floor((int)(5 + (camera.Pitch + 33) / 11) * cos(glm::radians(90 - camera.Yaw)));
		Block c(x, y, z,orange);
		vec3 positionCam(camera.Position.x, camera.Position.y, camera.Position.z);

		for (int i = 0; i < entities.size(); i++)
		{
			glm::vec3 origin = entities[i].chunk.origin;

			int disx = (int)abs(origin.x - camera.Position.x);
			int disz = (int)abs(origin.z - camera.Position.z);

			if (disx <= 32 && disz <= 32) {
				Chunk chunk = entities[i].chunk;
				vec3 h;
				int indice = -1;
				float dist = 100;
				int q = 0;
				ray vision =ray(point3(camera.Position.x, camera.Position.y, camera.Position.z), vec3(camera.Front.x, camera.Front.y, camera.Front.z));
				for (auto b : chunk.blocks) {
					//std::cout << vec3(b.x + chunk.origin.x, b.y , b.z + chunk.origin.z) << " " <<  point3(c.x, c.y, c.z) << " " << vec3(camera.Front.x, camera.Front.y, camera.Front.z) << std::endl;
					vec3 hit = intersect(vision, vec3(b.x+ chunk.origin.x, b.y, b.z + chunk.origin.z));
					if (vec3(0, 0, 0) != hit) {
						float d = (vision.at(hit.x()) - positionCam).length_squared();
						if (dist > d) {
							dist = d;
							indice = q;
							h = hit;
						}
					}
					q++;
				}
				if (indice != -1)
					chunk.blocks.erase(chunk.blocks.begin() + indice);
				auto v1 = chunk.blocks.end();//std::remove_if(chunk.blocks.begin(), chunk.blocks.end(), [c,chunk](Block i) { return  (c.x == (i.x + chunk.origin.x)) && (i.y == c.y) && (c.z == (i.z + chunk.origin.z));  });
				if (indice != -1) {
					flag = i;
					entities[i].update(chunk);
					//std::cout << "encontrei";
					break;
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
		std::cout << sizeof(float) * entities[flag].positionsList.size() << std::endl;
		std::cout << entities[flag].chunk.blocks[entities[flag].chunk.blocks.size()-1].x << " " << entities[flag].chunk.blocks[entities[flag].chunk.blocks.size()-1].y << " " << entities[flag].chunk.blocks[entities[flag].chunk.blocks.size()-1].z << std::endl;
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