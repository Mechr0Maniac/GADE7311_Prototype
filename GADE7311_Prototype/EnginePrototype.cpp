//Sean Martin 19011648 & Tayla Roach 18022560

/*References
   Open source files prvided by:
   
   OpenGl: de Vries, J. 2014. Learn OpenGL. [Online]. Available at: https://learnopengl.com/ (Accessed 28 September 2021)
   glad: Dav1dde. 2021. Glad. [Online]. Available at: https://glad.dav1d.de/ (Accessed 28 September 2021)
   GLFW: GLFW. 2021. GLFW. [Online]. Available at: https://www.glfw.org/ (Accessed 28 September 2021)
   nothings stb: Barrette, S. 2021. nothings stb. [Online]. Available at: https://github.com/nothings/stb (Accessed 29 September 2021)
   glm: G-Truc Creation. 2017. OpenGL Mathematics. [Online]. Available at: https://glm.g-truc.net/0.9.8/index.html (Accessed 29 September 2021)
*/


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine_Shaders.h"
#include "stb_image.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

const unsigned int WIDTH = 900;
const unsigned int HEIGHT = 900;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 70.0f;
float deltaTime = 0.0f;	
float lastFrame = 0.0f;
float lastX = WIDTH/2, lastY = HEIGHT/2;
bool firstMouse = true;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* engineView = glfwCreateWindow(WIDTH, HEIGHT, "Sean Martin 19011648 & Tayla Roach 18022560 Game Engine", NULL, NULL);
    if (engineView == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(engineView);
    glfwSetFramebufferSizeCallback(engineView, framebuffer_size_callback);
    glfwSetCursorPosCallback(engineView, mouse_callback);
    glfwSetScrollCallback(engineView, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //gltools
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(engineView, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //shaders
    EngineShader newShaders("ShaderVertex.txt", "ShaderFragment.txt");

    //vertices
    float room1[] = {
         // positions         // colours         //textures
        //wall 1
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //
         0.1f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.7f, 0.0f, // tri 1
         0.1f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.7f, 1.0f, //
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // tri 2
         0.1f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.7f, 1.0f, //
         0.1f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.7f, 1.0f, //
         0.4f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.9f, 1.0f, // tri 3
         0.1f,  0.1f, -0.5f,  0.0f, 0.0f, 1.0f,  0.7f, 0.6f, //
         0.4f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.9f, 1.0f, //
         0.4f,  0.1f, -0.5f,  0.0f, 0.0f, 1.0f,  0.9f, 0.6f, // tri 4
         0.1f,  0.1f, -0.5f,  0.0f, 0.0f, 1.0f,  0.7f, 0.6f, //
         0.4f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.9f, 1.0f, //
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // tri 5
         0.4f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.9f, 0.0f, //
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // tri 6
         0.4f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.9f, 0.0f, //

         //wall 2
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // tri 7
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, //
         0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,  0.0f, 1.0f, //
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, // tri 8

        //wall 3
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // tri 9
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // 
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, //
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,  0.0f, 1.0f, // tri 10
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, //
        
         //wall 4
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // tri 11
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, //
         0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,  0.0f, 1.0f, // tri 12
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, //

         //wall 5
        -0.5f, -0.5f, -1.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //
         0.5f, -0.5f, -1.50f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // tri 13
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, //
        -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,  0.0f, 0.0f, // tri 14
        -0.5f, -0.5f, -1.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, //

        //wall 6
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // tri 15
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, //
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,  0.0f, 1.0f, // tri 16
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f  //
    };
    float door[]{
         0.1f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //
         0.4f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // tri 1
         0.4f,  0.1f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, //

         0.1f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //
         0.1f,  0.1f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, // tri 2
         0.4f,  0.1f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f  //
    };

    glm::vec3 cubePositions[] = {
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    //glm::vec3(-1.7f,  3.0f, -7.5f),
    //glm::vec3(1.3f, -2.0f, -2.5f),
    //glm::vec3(1.5f,  2.0f, -2.5f),
    //glm::vec3(1.5f,  0.2f, -1.5f),
    //glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    //textures
    int texWdh, texHgt, texChan;
    unsigned char* texture1 = stbi_load("tex_wood_planks.jpg", &texWdh, &texHgt, &texChan, 0);
    unsigned char* texture2 = stbi_load("tex_brick_blocks.jpg", &texWdh, &texHgt, &texChan, 0);

    unsigned int texture[2];
    glGenTextures(2, texture);



    unsigned int VBO[2];
    glGenBuffers(2, VBO); 
    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    //room
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(room1), room1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWdh, texHgt, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture1);

    //door
    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(door), door, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWdh, texHgt, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture2);

    //wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //rendering
    while (!glfwWindowShouldClose(engineView))
    {
        //input
        processInput(engineView);
        //render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //texture
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        //draw
        newShaders.use();

        glm::mat4 object = glm::mat4(1.0f);
        glm::mat4 door = glm::mat4(1.0f);
        glm::mat4 camera(1.0f);
        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        camera = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 perspective;
        perspective = glm::perspective(glm::radians(fov), 900.0f / 900.0f, 0.1f, 100.0f);

        unsigned int objCoord = glGetUniformLocation(newShaders.ShaderID, "object");
        glUniformMatrix4fv(objCoord, 1, GL_FALSE, glm::value_ptr(object));
        unsigned int cmrCoord = glGetUniformLocation(newShaders.ShaderID, "camera");
        glUniformMatrix4fv(cmrCoord, 1, GL_FALSE, &camera[0][0]);
        newShaders.setMat4("perspective", perspective);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glBindVertexArray(VAO[0]);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 48);
        for (unsigned int i = 0; i < 5; i++)
        {
            object = glm::translate(object, cubePositions[i]);
            //float angle = 20.0f * i;
            // = glm::rotate(object, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
            newShaders.setMat4("object", object);

            glDrawArrays(GL_TRIANGLES, 0, 48);
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glBindVertexArray(VAO[1]);
        for (unsigned int i = 0; i < 5; i++)
        {
            door = glm::translate(door, cubePositions[i]);
            //float angle = 20.0f * i;
            // = glm::rotate(object, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
            newShaders.setMat4("object", door);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        //buffers
        glfwSwapBuffers(engineView);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 4.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
};

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset * 4.0f;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 70.0f)
        fov = 70.0f;
}