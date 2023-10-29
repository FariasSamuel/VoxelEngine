#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Game.h"
#include <thread>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;


void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float lastX = 400, lastY = 300;
bool firstMouse = true;

Camera camera(glm::vec3(0.0f,0.0f,0.0f));

int idx = 0;
int idy = 0;
float lastTime = glfwGetTime();

int posx = 10;
int posz = 10;

int colors = 0;

std::vector<int> v[3];
std::vector<float> cor[3];

int get(float i) {
    int t = i * 1000;
    std::cout << abs(t % 100 - t % 10) << std::endl;
    if (abs(t % 100-t % 10 )/10> 5) {
        return (int)i + 1;
    }
    else {
        return (int)i ;
    }
}

std::vector<float> c[4] = {
    {1.0f, 0.5f, 0.2f, 1.0f},
    {1.0f, 0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.f, 1.0f},
    {0.0f, 0.0f, 1.0f, 1.0f},
};

bool procurar() {
    int isnt = false;
    for (int i = 0; i < v[0].size(); i++) {
        if (v[0][i] == (int)camera.Position.x + floor((int)(5 + (camera.Pitch + 33) / 11) * sin(glm::radians(90 - camera.Yaw))) && v[1][i] == (int)camera.Position.z + floor((int)(5 + (camera.Pitch + 33) / 11) * cos(glm::radians(90 - camera.Yaw)))) {
            isnt = true;
            break;
        }
    }
    return isnt;
}
Game game(800, 600);
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

   
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    game.Init();

    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;

    std::thread gen1(&Game::generation,&game,true);
    std::thread gen2(&Game::generation, &game, false);
    std::thread destroy(&Game::destroier, &game,  window);

    while (!glfwWindowShouldClose(window))
    {
        
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;
        if (timeDiff >= 1.0 / 30.0) {
            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::cout << FPS << " " << ms << " " << game.entities.size() << std::endl;
            prevTime = crntTime;
            counter = 0;
        }

        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK,
            GL_LINE);
        game.Update(timeDiff);
        game.Render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ResourceManager::Clear();
    glfwTerminate();
    gen1.join();
    gen2.join();
    destroy.join();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        game.camera.processInput(FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        game.camera.processInput(BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        game.camera.processInput(LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        game.camera.processInput(RIGHT);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetTime()-lastTime  > 1) {
        (colors< 3) ? colors++: colors = 3;
        lastTime = glfwGetTime();
    }
        
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && glfwGetTime() - lastTime > 1) {
        (colors > 0) ? colors-- : colors = 0;
        lastTime = glfwGetTime();
    }
        
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetTime() - lastTime > 1) {
        idy++;
        lastTime = glfwGetTime();
    }
        
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && glfwGetTime() - lastTime > 1) {
        (idx > 0) ? idy-- : idy = 0;
        lastTime = glfwGetTime();
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && glfwGetTime() - lastTime > 0.25) {
        posx++;
        lastTime = glfwGetTime();
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetTime() - lastTime > 0.25) {
        posz--;
        lastTime = glfwGetTime();
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && glfwGetTime() - lastTime > 0.25) {
        posx--;
        lastTime = glfwGetTime();
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && glfwGetTime() - lastTime > 0.25) {
        posz++;
        lastTime = glfwGetTime();
    }
    
        
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    game.camera.mouse_input(xoffset, yoffset);
}