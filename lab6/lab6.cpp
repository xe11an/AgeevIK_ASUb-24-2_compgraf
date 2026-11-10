#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Model.h"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
bool firstMouse = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
float cameraSpeed = 0.05f;
float mouseSensitivity = 0.1f;

float angle1 = 0.0f;
float angle2 = 0.0f;
float angle3 = 0.0f;

const float ANGLE_STEP = 0.02f;
const float MAX_ANGLE = 3.14f;
const float MIN_ANGLE = -3.14f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos; lastY = ypos;
    xoffset *= mouseSensitivity; yoffset *= mouseSensitivity;
    yaw += xoffset; pitch += yoffset;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lab7 - Affine Transformations", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glEnable(GL_DEPTH_TEST);

    Model ourModel("models/lab3_ageev.obj");
    Shader ourShader("shaders/shader.vert", "shaders/shader.frag");

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { angle1 -= ANGLE_STEP; if (angle1 < MIN_ANGLE) angle1 = MIN_ANGLE; }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { angle1 += ANGLE_STEP; if (angle1 > MAX_ANGLE) angle1 = MAX_ANGLE; }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) { angle2 += ANGLE_STEP; if (angle2 > MAX_ANGLE) angle2 = MAX_ANGLE; }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) { angle2 -= ANGLE_STEP; if (angle2 < MIN_ANGLE) angle2 = MIN_ANGLE; }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) { angle3 += ANGLE_STEP; if (angle3 > MAX_ANGLE) angle3 = MAX_ANGLE; }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) { angle3 -= ANGLE_STEP; if (angle3 < MIN_ANGLE) angle3 = MIN_ANGLE; }

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

        ourShader.setVec3("material.ambient", 0.3f, 0.3f, 0.3f);
        ourShader.setVec3("material.diffuse", 1.0f, 0.8f, 0.4f);
        ourShader.setVec3("material.specular", 0.8f, 0.8f, 0.8f);
        ourShader.setFloat("material.shininess", 32.0f);

        ourShader.setVec3("light.position", 2.0f, 2.0f, 2.0f);
        ourShader.setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
        ourShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        
        float s = 0.1f;
        glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(s));

       
        glm::mat4 m1 = glm::rotate(glm::mat4(1.0f), angle1, glm::vec3(0.0f, 1.0f, 0.0f));

       
        glm::vec3 P2(0.53f, 1.1f, 0.0f);
        glm::mat4 m2 = m1
            * glm::translate(glm::mat4(1.0f), P2)
            * glm::rotate(glm::mat4(1.0f), angle2, glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::translate(glm::mat4(1.0f), -P2);

        
        glm::vec3 P3(1.53f, 1.1f, 0.0f);
        glm::mat4 m3 = m2
            * glm::translate(glm::mat4(1.0f), P3)
            * glm::rotate(glm::mat4(1.0f), angle3, glm::vec3(0.0f, 0.0f, 1.0f)) 
            * glm::translate(glm::mat4(1.0f), -P3);

        glm::mat4 baseMatrix = S;
        glm::mat4 x01Matrix = S * m1;
        glm::mat4 x02Matrix = S * m2;
        glm::mat4 x03Matrix = S * m3;

        unsigned int meshCount = ourModel.getMeshCount();

        for (unsigned int i = 0; i < meshCount; i++) {
            glm::mat4 modelMatrix;

            switch (i) {
            case 0: modelMatrix = baseMatrix; break;
            case 1: modelMatrix = x01Matrix; break;
            case 2: modelMatrix = x02Matrix; break;
            case 3: modelMatrix = x03Matrix; break;
            default: modelMatrix = baseMatrix; break;
            }

            ourModel.DrawMesh(i, ourShader, modelMatrix);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}