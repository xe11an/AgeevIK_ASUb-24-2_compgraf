#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include <cstdio>
#include <cmath>

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3.\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);   // если окно не создаётся - поставьте 1 (версия 4.1)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(512, 512, "Mainwindow", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not create window (check OpenGL version).\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum ret = glewInit();
    if (GLEW_OK != ret) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(ret));
        return 1;
    }

    printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("This version OpenGL running is %s\n", glGetString(GL_VERSION));
    printf("This device OpenGL running is %s\n", glGetString(GL_RENDERER));

    // ---------- данные пятиугольника (вариант 5) ----------
    float points[] = {
         0.0000f,  0.6000f, 0.0f,   // V0
        -0.5706f,  0.1854f, 0.0f,   // V1
        -0.3527f, -0.4854f, 0.0f,   // V2
         0.3527f, -0.4854f, 0.0f,   // V3
         0.5706f,  0.1854f, 0.0f    // V4
    };
    GLuint indices[] = {            // 3 треугольника, нумерация с 0
        0, 1, 2,
        0, 2, 3,
        0, 3, 4
    };

    // ---------- VAO, VBO, EBO ----------
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);            // EBO отвязывать до этого нельзя

    // ---------- шейдеры из файлов (рабочая папка = папка проекта) ----------
    Shader shader("shaders/shader.vert", "shaders/shader.frag");
    if (!shader.valid()) {
        glfwTerminate();
        return 1;
    }

    // ---------- движение ----------
    float posX = 0.0f, posY = 0.0f;
    bool autoMove = true;            // пробел переключает авто/ручное
    bool spaceWasDown = false;
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        float dt = (float)(now - lastTime);
        lastTime = now;
        float t = (float)now;

        bool spaceDown = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        if (spaceDown && !spaceWasDown) autoMove = !autoMove;
        spaceWasDown = spaceDown;

        if (autoMove) {
            posX = 0.3f * std::cos(t);       // движение по кругу
            posY = 0.3f * std::sin(t);
        } else {
            const float speed = 0.8f;        // WASD или стрелки
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    posY += speed * dt;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  posY -= speed * dt;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  posX -= speed * dt;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) posX += speed * dt;
        }
        // не даём фигуре выйти за окно
        posX = std::fmax(-0.42f, std::fmin(0.42f, posX));
        posY = std::fmax(-0.51f, std::fmin(0.40f, posY));

        // ---------- цвет от времени (задание №1) ----------
        float r = std::sin(t)        * 0.5f + 0.5f;
        float g = std::sin(t + 2.1f) * 0.5f + 0.5f;
        float b = std::sin(t + 4.2f) * 0.5f + 0.5f;

        // ---------- кадр ----------
        glClearColor(0.8f, 0.2f, 0.7f, 1.0f);   // фон варианта 5
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setVec2("offset", posX, posY);
        shader.setVec4("ourColor", r, g, b, 1.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);   // 3 треугольника = 9 индексов

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    shader.destroy();
    glfwTerminate();
    return 0;
}
