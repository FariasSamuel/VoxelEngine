#include "game.h"
#include <vector>


Game::Game(unsigned int width, unsigned int height): Keys(), Width(width), Height(height)
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

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    projection = pespective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 100.0f);

    this->modelLoc = glGetUniformLocation(s.ID, "model");
    this->viewLoc = glGetUniformLocation(s.ID, "view");
    this->projectionLoc = glGetUniformLocation(s.ID, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glEnable(GL_DEPTH_TEST);

}

bool Game::perto(std::pair<int,int> i) {
    
    return (abs(i.first - camera.Position.x) > WORLD_SIZE) || (abs(i.second - camera.Position.z) > WORLD_SIZE);
}
/*
void Game::destroier(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        std::lock_guard<std::mutex> guard(g_pages_mutex);
        //std::vector<std::pair<int, int>>::iterator v;
        //std::cout << "nya" << std::endl;
        auto v = std::remove_if(entities.begin(), entities.end(), [this](const std::pair<int, int> i) { return (abs(i.first - camera.Position.x) > WORLD_SIZE) || (abs(i.second - camera.Position.z) > WORLD_SIZE); });
        v = std::remove_if(used.begin(), used.end(), [this](const std::pair<int, int> i) { return (abs(i.first - camera.Position.x) > WORLD_SIZE) || (abs(i.second - camera.Position.z) > WORLD_SIZE); });
        for (auto i: entities) {
            int disx = abs(i.first - camera.Position.x);
            int disz = abs(i.second - camera.Position.z);

            if (disx > 16 || disz > 16) {
                v.push_back(i);
            }

        }        
        for (int i = 0; i < v.size(); i++) {
            
            entities.remove(v[i]);
            used.remove(v[i]);
        }
    }
}*/

void Game::generation(bool a) {
    while (true) {
        //std::cout << "hello" << std::endl;
       
            std::lock_guard<std::mutex> guard(g_pages_mutex);
            try {
                for (int i = (int)(this->camera.Position.x - 32) / 16; i < (int)(this->camera.Position.x + 32) / 16; i++) {
                    for (int j = (int)(this->camera.Position.z - 32) / 16; j < (int)(this->camera.Position.z + 32) / 16; j++) {
                        if (std::find(used.begin(), used.end(), std::make_pair(i * 16, j * 16)) == used.end()) {
                            std::vector<std::pair<int, int>> blocks;

                            for (int x = 0; x < 16; x++) {
                                for (int y = 0; y < 16; y++) {
                                    blocks.push_back(std::make_pair((i * 16) + x, (j * 16) + y));
                                }
                            }
                            Chunk c(blocks, glm::vec3(i * 16, 0 * 16, j * 16));
                            entities.push_back(c);
                            this->used.push_back(std::make_pair(i * 16, j * 16));
                        }
                    }
                }
            }
            catch (const std::exception& oor) {
                std::cerr << "Out of Range error: " << oor.what() << '\n';
            }
          
        
    }
}

bool Game::procurar(int x, int z) {
    for (auto i : used) {
        if (i.first == x  && i.second == z ) {
            return true;
        }
    }
    return false;
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
    //std::cout << "nyaaa" << std::endl;
    view = camera.getView();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    s.Use();
    glBindVertexArray(VAO);
    try {
    //std::lock_guard<std::mutex> guard(g_pages_mutex);
    for (int i = 0; i < entities.size(); i++)
    {
        glm::vec3 origin = entities[i].origin;
        std::cout << entities.size() << std::endl;
        int disx = (int)abs(origin.x - camera.Position.x);
        int disz = (int)abs(origin.z - camera.Position.z);

        if (disx <= 32 && disz <= 32) {
        
            for (int k = 0; k < entities[i].blocks.size(); k++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(entities[i].blocks[k].first, 0, entities[i].blocks[k].second));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
 

        }

    }
}
    catch (const std::exception& oor) {
        std::cerr << "Out of Range error: " << oor.what() << '\n';
    }

    s.Use();
}