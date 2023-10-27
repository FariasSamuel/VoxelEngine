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
    return (abs(i.first - camera.Position.x) > 10) || (abs(i.second - camera.Position.z) > 10);
}

void Game::destroier() {
    while (true) {
        std::lock_guard<std::mutex> guard(g_pages_mutex);
        std::vector<std::pair<int, int>> v;
        std::remove_if(entities.begin(), entities.end(), perto);
        std::remove_if(used.begin(), used.end(), perto);
        /*for (auto i: entities) {
            int disx = abs(i.first - camera.Position.x);
            int disz = abs(i.second - camera.Position.z);

            if (disx > 16 || disz > 16) {
                v.push_back(i);
            }

        }        
        for (int i = 0; i < v.size(); i++) {
            
            entities.remove(v[i]);
            used.remove(v[i]);
        }*/
    }
}

void Game::generation(bool a) {
    while (true) {
        //std::cout << "hello" << std::endl;
       
            std::lock_guard<std::mutex> guard(g_pages_mutex);
            if (a == true) {
                for (int i = (int)this->camera.Position.x - 10; i < (int)this->camera.Position.x; i++) {
                    for (int j = (int)this->camera.Position.z - 10; j < (int)this->camera.Position.z; j++) {
                        if (!procurar(i, j)) {
                            this->entities.push_back(std::make_pair(i, j));
                            this->used.push_back(std::make_pair(i, j));
                        }
                    }
                }
                for (int i = (int)this->camera.Position.x - 10; i < (int)this->camera.Position.x; i++) {
                    for (int j = (int)this->camera.Position.z; j < (int)this->camera.Position.z + 10; j++) {
                        if (!procurar(i, j)) {
                            this->entities.push_back(std::make_pair(i, j));
                            this->used.push_back(std::make_pair(i, j));
                        }
                    }
                }
            }
            else {
                for (int i = (int)this->camera.Position.x; i < (int)this->camera.Position.x + 10; i++) {
                    for (int j = (int)this->camera.Position.z - 10; j < (int)this->camera.Position.z; j++) {
                        if (!procurar(i, j)) {
                            this->entities.push_back(std::make_pair(i,j));
                            this->used.push_back(std::make_pair(i, j));
                        }
                    }
                }
                for (int i = (int)this->camera.Position.x; i < (int)this->camera.Position.x + 10; i++) {
                    for (int j = (int)this->camera.Position.z; j < (int)this->camera.Position.z + 10; j++) {
                        if (!procurar(i, j)) {
                            this->entities.push_back(std::make_pair(i, j));
                            this->used.push_back(std::make_pair(i, j));
                        }
                    }
                }
            
        
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
    std::lock_guard<std::mutex> guard(g_pages_mutex);
    for (int i = 0; i < entities.size();i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(i.first, 0, i.second));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    s.Use();
}