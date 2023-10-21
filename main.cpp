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

#include<vector>

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
    Shader s("shader.vs","shader.fs");
    s.use();
    
    glEnable(GL_DEPTH_TEST);



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
    

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);    

    glm::mat4 model = glm::mat4(1.0f); 
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    
    projection = pespective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    unsigned int modelLoc = glGetUniformLocation(s.ID, "model");
    unsigned int viewLoc = glGetUniformLocation(s.ID, "view");
    unsigned int projectionLoc = glGetUniformLocation(s.ID, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    float x = camera.Position.x - 5;
    float y = camera.Position.y - 3;
    float z = camera.Position.z - 5;

    int max = 10;
    float lim = x+10;
    float piso = x;

    unsigned int colorLoc = glGetUniformLocation(s.ID, "color");
 


    while (!glfwWindowShouldClose(window))
    {
        

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK,
        //    GL_LINE);
        
        s.use();
        float radius = 10.0f;
        float camX = static_cast<float>(sin(glfwGetTime()) * radius);
        float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
        
        view = camera.getView();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        s.use();
        glBindVertexArray(VAO);
        std::cout << (int)camera.Position.x + ((int)(5 + (camera.Pitch + 33) / 11) * sin(glm::radians(90 - camera.Yaw))) << " " << y + 1 << (int)camera.Position.z + ((int)(5 + (camera.Pitch + 33) / 11) * cos(glm::radians(90 - camera.Yaw))) << std::endl;
        //std::cout << (int)camera.Position.x + (int)(10 * camera.Front.x) << " " << (int)camera.Position.z + (camera.Front.z / abs(camera.Front.z)) * (10 + (int)(10 * camera.Front.y)) << std::endl;
        if (camera.Position.x > lim ) {
            max += 10;
            lim += 10;
        }
        if (camera.Position.x < piso) {
            idx -= 10;
            piso -= 10;
        }
        for ( int i = idx; i < max; i++)
        {
            for ( int j = idy; j < 10; j++)
            {
                    glUniform4f(colorLoc, 1.0f, 0.5f, 0.2f, 1.0f);
                    glm::mat4 model = glm::mat4(1.0f); 
                    model = glm::translate(model, glm::vec3(i+x,y,j + z));
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                
            }
        }
        //m = glm::translate(m, glm::vec3((int)camera.Position.x + floor(ceil(6 * sin(glm::radians(90 - camera.Pitch))) * sin(glm::radians(90 - camera.Yaw))) , y + 1, (int)camera.Position.z + floor(ceil(6 * sin(glm::radians(90 - camera.Pitch))) * cos(glm::radians(90 - camera.Yaw)))));
        glm::mat4 m = glm::mat4(1.0f);
        glUniform4f(colorLoc, 2.0f, 0.0f, 0.0f, 1.0f);
        int isnt = procurar();
        if (!isnt) {
            m = glm::translate(m, glm::vec3((int)camera.Position.x + floor((int)(5 + (camera.Pitch + 33) / 11) * sin(glm::radians(90 - camera.Yaw))), y + 1, (int)camera.Position.z + floor((int)(5 + (camera.Pitch + 33) / 11) * cos(glm::radians(90 - camera.Yaw)))));
        }
        else {
            m = glm::translate(m, glm::vec3((int)camera.Position.x + floor((int)(5 + (camera.Pitch + 33) / 11) * sin(glm::radians(90 - camera.Yaw))), y + 2, (int)camera.Position.z + floor((int)(5 + (camera.Pitch + 33) / 11) * cos(glm::radians(90 - camera.Yaw)))));

        }
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        for (int i = 0; i < v[0].size(); i++)
        {
            glm::mat4 mod = glm::mat4(1.0f);
            mod = glm::translate(mod, glm::vec3(v[0][i], y + 1, v[1][i]));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mod));
            glUniform4f(colorLoc, cor[0][i], cor[1][i], cor[2][i] , 1.0f);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        s.use();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(s.ID);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processInput(FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processInput(BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processInput(LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processInput(RIGHT);
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
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && glfwGetTime() - lastTime > 0.25) {
        auto x = v[0].begin();
        auto y = v[1].begin();
        for (int i = 0; i < v[0].size(); i++) {
            if (v[0][i] == (int)camera.Position.x + floor((int)(5 + (camera.Pitch + 33) / 11) * sin(glm::radians(90 - camera.Yaw))) && v[1][i] == (int)camera.Position.z + floor((int)(5 + (camera.Pitch + 33) / 11) * cos(glm::radians(90 - camera.Yaw)))) {
                v[0].erase(x);
                v[1].erase(y);
                break;
            }
            else {
                std::advance(x, 1);
                std::advance(y, 1);
            }
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && glfwGetTime() - lastTime > 0.25) {
        int isnt = procurar();
        if (!isnt) {
            v[0].push_back((int)camera.Position.x + floor((int)(5 + (camera.Pitch + 33) / 11) * sin(glm::radians(90 - camera.Yaw))));
            v[1].push_back((int)camera.Position.z + floor((int)(5 + (camera.Pitch + 33) / 11) * cos(glm::radians(90 - camera.Yaw))));
            cor[0].push_back(c[0][colors]);
            cor[1].push_back(c[1][colors]);
            cor[2].push_back(c[2][colors]);
        }
        lastTime = glfwGetTime();
    }
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

    camera.mouse_input(xoffset, yoffset);
}