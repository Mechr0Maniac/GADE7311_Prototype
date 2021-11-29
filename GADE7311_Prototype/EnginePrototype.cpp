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
#include "Model_Loader.h"
#include "stb_image.h"
#include <string>
#include <vector>
#include <time.h>
#include <filesystem>
#include "SkyboxShader.h"

#include <iostream>
#include "Camera.h"

using std::string;
using std::vector;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void character_callback(GLFWwindow* window, unsigned int codepoint);

string ConsoleCommands();
vector<string> split(string x, char delim = ' ');

const unsigned int WIDTH = 900;
const unsigned int HEIGHT = 900;

//Camera settings
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 70.0f;
float lastX = WIDTH/2, lastY = HEIGHT/2;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool consoleEnabled = false;
bool consoleTyping = false;
const char Console_key = GLFW_KEY_SPACE;
string command_line;
string previousCommands;
//string path = "level.fbx";
int startTime = time(NULL);
int endTime;
int fps = 0;
int fpsCount = 0;
//Model models(path);

GLFWwindow* engineView;

double lastTime = glfwGetTime();
int nbFrames = 0;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    engineView = glfwCreateWindow(WIDTH, HEIGHT, "Sean Martin 19011648 & Tayla Roach 18022560 Game Engine", NULL, NULL);
    
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
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(engineView, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //shaders
    SkyboxShaderClass newShaders("ShaderVertex.vert", "ShaderFragment.frag");
    SkyboxShaderClass skyboxShader("SkyBoxVertexShader.vert", "SkyBoxFragmentShader.frag");

    //Models
    //Model models(path);
    //Model models("backpack.obj");
    Model models("WolfensteinMap.obj");
    //Model models("Survival_BackPack_2.fbx");
    //Model models("level.fbx");
    //Model models("leve.obj");

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

    float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };

    unsigned int skyboxIndices[] =
    {
        // Right
        1, 2, 6,
        6, 5, 1,
        // Left
        0, 4, 7,
        7, 3, 0,
        // Top
        4, 5, 6,
        6, 7, 4,
        // Bottom
        0, 3, 2,
        2, 1, 0,
        // Back
        0, 1, 5,
        5, 4, 0,
        // Front
        3, 7, 6,
        6, 2, 3
    };

    /*//textures
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
    stbi_image_free(texture2);*/

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::string facesCubemap[6] =
    {
        "textures/right.jpg",
        "textures/left.jpg",
        "textures/top.jpg",
        "textures/bottom.jpg",
        "textures/front.jpg",
        "textures/back.jpg"
    };

    unsigned int cubemapTexture;

    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Cycles through all the textures and attaches them to the cubemap object
    for (unsigned int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
            stbi_image_free(data);
        }
    }

    newShaders.Activate();
    glUniform1i(glGetUniformLocation(newShaders.ID, "texture1"), 0);

    skyboxShader.Activate();
    glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);
    
    //wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // setting key callback method
    glfwSetKeyCallback(engineView, key_callback);
    glfwSetCharCallback(engineView, character_callback);

    

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
        //glBindTexture(GL_TEXTURE_2D, texture[0]);
        
        
        //draw
        newShaders.Activate();
        glm::mat4 object = glm::mat4(1.0f);
        glm::mat4 door = glm::mat4(1.0f);

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        glm::mat4 cameraValue(1.0f);

        view = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
        projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        cameraValue = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 perspective;
        perspective = glm::perspective(glm::radians(fov), 900.0f / 900.0f, 0.1f, 100.0f);

        

        //model render
        object = glm::translate(object, glm::vec3(0.0f, 0.0f, 0.0f)); 
        object = glm::scale(object, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(newShaders.ID, "object"), 1, GL_FALSE, glm::value_ptr(object));
        models.Draw(newShaders);

        unsigned int objCoord = glGetUniformLocation(newShaders.ID, "object");
        glUniformMatrix4fv(objCoord, 1, GL_FALSE, glm::value_ptr(object));
        unsigned int cmrCoord = glGetUniformLocation(newShaders.ID, "camera");
        glUniformMatrix4fv(cmrCoord, 1, GL_FALSE, &cameraValue[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(newShaders.ID, "perspective"), 1, GL_FALSE, glm::value_ptr(perspective));

        glDepthFunc(GL_LEQUAL);

        skyboxShader.Activate();

        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
        // where an object is present (a depth of 1.0f will always fail against any object's depth value)
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Switch back to the normal depth function
        glDepthFunc(GL_LESS);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        
        
        //glBindVertexArray(VAO[0]);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 48);
        /*for (unsigned int i = 0; i < 5; i++)
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
            // = glm::rotate(object, glm::radians
            
            (angle), glm::vec3(1.0f, 1.0f, 1.0f));
            newShaders.setMat4("object", door);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }*/
        //buffers
        glfwSwapBuffers(engineView);
        glfwPollEvents();

        //FPS Count
        fps++;
        endTime = time(NULL);

        if (endTime - startTime > 0)
        {
            fpsCount = fps;

            fps = 0;
            startTime = endTime;
        }

        // Measure speed for FPS
        /*double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
            // printf and reset timer
            fps = nbFrames;

            nbFrames = 0;
            lastTime += 1.0;
        }*/
    }

    glfwTerminate();
    return 0;
}

/*unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_PROXY_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_PROXY_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_PROXY_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_PROXY_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_PROXY_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}*/


void processInput(GLFWwindow* window)
{
    // setting key callback method
    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed = 2.5 * deltaTime;

    if (!consoleEnabled) 
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    /*if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    if (!consoleEnabled) {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }*/
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
};

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(yoffset);
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == Console_key)
    {
        if (action == GLFW_PRESS)
        {
            if (!consoleEnabled)
            {
                consoleEnabled = true;
                glfwSetInputMode(engineView, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                printf("\nConsole Enabled\n");
            }
            else
            {
                if (command_line.size() > 0)
                {
                    printf("\33[2K\r");
                }

                consoleEnabled = false;
                glfwSetInputMode(engineView, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                printf("Console Disabled");
            }
        }
    }
    else if (consoleEnabled)
    {
        if (action == GLFW_PRESS)
        {
            if (key == GLFW_KEY_ENTER)
            {
                consoleEnabled = false;
                consoleTyping = false;

                if (command_line.size() > 0)
                {
                    string executeCommand = ConsoleCommands();
                    printf(executeCommand.c_str());
                }
                return;
            }

            if (key == GLFW_KEY_BACKSPACE)
            {
                if (command_line.size() > 0)
                {
                    printf("\33[2K\r");
                    command_line.resize(command_line.size() - 1);
                    printf(command_line.c_str());
                }
                return;
            }
        }
    }
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    if (codepoint != Console_key)
    {
        if (consoleEnabled)
        {
            consoleTyping = true;

            char c = static_cast<char>(codepoint);
            command_line += c;
            printf("\33[2K\r");
            printf(command_line.c_str());
        }
                
    }
}


string ConsoleCommands()
{
    try //error handling...yay
    {
        vector<string> command = split(command_line);

        if (command[0] == "fps")
        {
            printf("\n%f fps", 1 / deltaTime);

            return "\nExecuted: " + command[0];
        }

        else if (command[0] == "object_load")
        {
            return "\nExecuted: " + command[0];
        }

        else if (command[0] == "level_load")
        {
            return "\nExecuted: " + command[0];
        }

        else if (command[0] == "triangle_count")
        {
            return "\nExecuted: " + command[0];
        }

        else if (command[0] == "help")
        {

            printf("\nCommands:");
            printf("\nfps - frames per second counter");
            printf("\nobject_load - load an object from a file");
            printf("\nlevel_load - load a level from a file");

            return "\nExecuted: " + command[0];
        }
        else
            throw "error";
    }
    catch (...)/*(const std::exception&)*/
    {
        return "\nError";
    }
}

vector<string> split(string x, char delim)
{
    x += delim; //includes a delimiter at the end so last word is also read
    vector<string> splitted;
    string temp = "";
    for (int i = 0; i < x.length(); i++)
    {
        if (x[i] == delim)
        {
            splitted.push_back(temp); //store words in "splitted" vector
            temp = "";
            i++;
        }
        temp += x[i];
    }
    return splitted;
}