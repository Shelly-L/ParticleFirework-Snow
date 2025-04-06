 #include"glad/glad.h"
#include"GLFW/glfw3.h"
#include <Windows.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#define GLM_ENABLE_EXPERIMENTAL
#include"glm/gtx/transform.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"glm/gtx/string_cast.hpp"
#include"glm/glm.hpp"
#include <iostream>
#include <map>
#include"geometry.h"
#include "camera.h"
#include "shader.h"
#include "utils.h"
#include "launcher.h"
#include "image-loader.h"
#include<cmath>
#include"texture.h"
#include"Skybox.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void updateCameraRotation(GLFWwindow* window);
void processInput(GLFWwindow* window);

float SCREEN_W = 1920.0f;
float SCREEN_H = 1080.0f;
float FOV = 65.0f;
float SENSITIVITY = 0.1f;

const float NEAR_CLIP = 1.0f;
const float FAR_CLIP = 1000.0f;

double lastXPos = SCREEN_W / 2, lastYPos = SCREEN_H / 2;
double yaw = 0, pitch = 0, xPos, yPos;
unsigned int Launcher::particlesCount = 0;

Camera* camera;
glm::highp_mat4 projection;
std::map<int, bool> heldKeys;





Geometry* cube = nullptr;
Shader* cubeShader = nullptr;


 float thera[]{
    0.000000, 0.209440,0.418879,0.628319,0.837758,1.047198,
    1.256637,1.466076,1.675516,1.884956,2.094395,2.303834,
    2.513274,2.722713,2.932153,3.141593,3.351032,3.560472,
    3.769911,3.979351,4.188790,4.398230,4.607669,4.817109,
    5.026548,5.235988,5.445427,5.654867,5.864306,6.073746
};
 float R = 50.f;

 // 旋转角度变量
 float angle = 0.0f;
 // 旋转轴，平行于y轴示例，这里通过起点和终点来表示一条直线方向作为旋转轴
 glm::vec3 axisStart(0.0f, 0.0f, 0.0f);
 glm::vec3 axisEnd(0.0f, 1.0f, 0.0f);
 glm::vec3 axis = normalize(axisEnd - axisStart);
 
 std::vector<std::string> faces
 {
    "skybox/posx.jpg",
    "skybox/negx.jpg",
    "skybox/posy.jpg",
    "skybox/negy.jpg",
    "skybox/posz.jpg",
    "skybox/negz.jpg"
 };




void prepareVAO() {
    cube = Geometry::createBox(5.0f);
}

void prepareShader() {
    cubeShader = new Shader("cube.vs", "cube.fs");
}



void prepareBOX() {
    glm::mat4 transform = glm::mat4(1.0f);
    

    //绑定当前的program
    cubeShader->use();
    cubeShader->setMatrix4x4("transform", transform);
    cubeShader->setMatrix4x4("viewMatrix", camera->getWorldToViewMatrix());
    cubeShader->setMatrix4x4("projectionMatrix", projection);

   

    //绑定当前的vao；
    glBindVertexArray(cube->getVao());
    for (int j = 0; j <=1; j++) {
        R = 40 + 3 * j;
        float Y;
        Y = 50 - 3 * j;
        for (unsigned int i = 0; i < 30; i++)
        {
            transform = glm::mat4(1.0f);
            float X, Z;
            X = R * cos(thera[i]);
            Z = R * sin(thera[i]);
            glm::vec3 place(X, Y, Z);
            transform = glm::rotate(transform, glm::radians(angle), axis);
            transform = glm::translate(transform, place);
            transform = glm::scale(transform, glm::vec3(0.2f)); // Make it a smaller cube
            cubeShader->setMatrix4x4("transform", transform);
            //发出绘画指令
            glDrawElements(GL_TRIANGLES, cube->getIndicesCount(), GL_UNSIGNED_INT, 0);
        }
    }

        R = 40 ;
        float Y;
        Y = 47 - 3;
        for (unsigned int i = 0; i < 30; i++)
        {
            transform = glm::mat4(1.0f);
            float X, Z;
            X = R * cos(thera[i]);
            Z = R * sin(thera[i]);
            glm::vec3 place(X, Y, Z);
            transform = glm::rotate(transform, glm::radians(angle), axis);
            transform = glm::translate(transform, place);
            transform = glm::scale(transform, glm::vec3(0.2f)); // Make it a smaller cube
            cubeShader->setMatrix4x4("transform", transform);
            //发出绘画指令
            glDrawElements(GL_TRIANGLES, cube->getIndicesCount(), GL_UNSIGNED_INT, 0);
        }
    



    glBindVertexArray(0);
}

void prepareCamera() {
    // Camera
    camera = new Camera(glm::vec3(-100.0f, 35.0f, -100.0f));
    projection = glm::perspective(glm::radians(FOV), (GLfloat)(SCREEN_W / SCREEN_H), NEAR_CLIP, FAR_CLIP);
}

int main()
{
    srand(time(NULL));

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Fireworks", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    prepareCamera();
    prepareShader();
    prepareVAO();
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    Skybox skybox(faces);

    // Texture
    ImageLoader img;
    Texture textureSnow("textures/snowflower.jpg",1);

    GLuint textureId = img.loadBMP_custom("textures/circle.bmp");

    // Shaders
    Shader particleShader("particle.vert", "particle.frag");
    GLuint u_textureId = glGetUniformLocation(particleShader.id, "sampler");

    GLuint cameraRightId = glGetUniformLocation(particleShader.id, "cameraRight");
    GLuint cameraUpId = glGetUniformLocation(particleShader.id, "cameraUp");
    GLuint viewProjId = glGetUniformLocation(particleShader.id, "VP");

    GLfloat* particle_position = new GLfloat[maxParticles * 4];
    GLubyte* particle_color = new GLubyte[maxParticles * 4];

    GLuint billboard_vertex_buffer;
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    GLuint particles_position_buffer;
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    GLuint particles_color_buffer;
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

    glBindVertexArray(0);

    Launcher launcher;
    int nbFrames = 0;
    double lastTime = glfwGetTime();

    glClearColor(0, 0.0f, 0.0f, 0.8f);
    while (!glfwWindowShouldClose(window))
    {
        nbFrames++;
        if (glfwGetTime() - lastTime >= 1.0) {
            printf("%i fps\n", nbFrames);
            nbFrames = 0;
            lastTime += 1.0;
        }

        Camera::updateDeltaTime();
        launcher.update(*camera, particle_position, particle_color);

        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //prepareBOX();

        
        glm::mat4 model(1.0f), view(1.0f);
        view = camera->getWorldToViewMatrix();
        skybox.Draw(projection, view);


        glEnable(GL_DEPTH_TEST);

        glBindVertexArray(VAO);

        // Projection
        auto viewMatrix = camera->getWorldToViewMatrix();
        auto vp = projection * viewMatrix;


      

        // Updating particle position buffer
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, Launcher::particlesCount * sizeof(GLfloat) * 4, particle_position);

        // Updating particle color buffer
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, Launcher::particlesCount * sizeof(GLubyte) * 4, particle_color);

        
        
        // Texture
        glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, textureId); 
        textureSnow.bind();
        glUniform1i(u_textureId, 0);



        glEnable(GL_BLEND);
        particleShader.use();

        // Shader uniforms
        glUniform3f(cameraRightId, viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
        glUniform3f(cameraUpId, viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
        glUniformMatrix4fv(viewProjId, 1, GL_FALSE, &vp[0][0]);

        // Quad vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Particle position
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Particle color
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

        glVertexAttribDivisor(0, 0); // Quad vertices (4 for each particle)
        glVertexAttribDivisor(1, 1); // 1 position for each quad
        glVertexAttribDivisor(2, 1); // 1 color for each quad

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, Launcher::particlesCount);
       

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        

        
        angle += 0.3f;

        glfwSwapBuffers(window);
        glfwPollEvents();

        

    }

    delete[] particle_position;
    delete[] particle_color;                    

    glDeleteBuffers(1, &particles_color_buffer);
    glDeleteBuffers(1, &particles_position_buffer);
    glDeleteBuffers(1, &billboard_vertex_buffer);
    glDeleteTextures(1, &u_textureId);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}

void updateCameraRotation(GLFWwindow* window)
{
    glfwGetCursorPos(window, &xPos, &yPos);

    yaw += (xPos - lastXPos) * SENSITIVITY;
    pitch += (lastYPos - yPos) * SENSITIVITY;
    pitch = clamp(pitch, 89.0f, -89.0f);

    lastXPos = xPos;
    lastYPos = yPos;

    camera->rotate((float)yaw, (float)pitch);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_REPEAT)
        return;
    heldKeys[key] = action == GLFW_PRESS;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera controls
    updateCameraRotation(window);

    if (heldKeys[GLFW_KEY_SPACE])
        camera->moveUp(2.0f);
    if (heldKeys[GLFW_KEY_LEFT_SHIFT])
        camera->moveUp(-2.0f);
    if (heldKeys[GLFW_KEY_W])
        camera->moveForward(2.0f);
    if (heldKeys[GLFW_KEY_S])
        camera->moveForward(-2.0f);
    if (heldKeys[GLFW_KEY_D])
        camera->moveLeft(1.5f);
    if (heldKeys[GLFW_KEY_A])
        camera->moveLeft(-1.5f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCREEN_W = (float)width;
    SCREEN_H = (float)height;
    projection = glm::perspective(glm::radians(FOV), (GLfloat)(SCREEN_W / SCREEN_H), NEAR_CLIP, FAR_CLIP);

    glViewport(0, 0, width, height);
}
