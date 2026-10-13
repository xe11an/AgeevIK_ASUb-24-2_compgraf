#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <cstdio>

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3.\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(512, 512, "Mainwindow", NULL, NULL);
    if (!window) {
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

    const GLubyte* version_str = glGetString(GL_VERSION);
    const GLubyte* device_str = glGetString(GL_RENDERER);
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("This version OpenGL running is %s\n", version_str);
    printf("This device OpenGL running is %s\n", device_str);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.8f, 0.2f, 0.7f, 1.0f);   // фон
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glColor3f(0.3f, 0.6f, 0.1f);            // цвет фигуры

        // V0 = ( 0.0000,  0.6000)
        // V1 = (-0.5706,  0.1854)
        // V2 = (-0.3527, -0.4854)
        // V3 = ( 0.3527, -0.4854)
        // V4 = ( 0.5706,  0.1854)

        glVertex2f(0.0000f, 0.6000f);   // V0
        glVertex2f(-0.5706f, 0.1854f);   // V1
        glVertex2f(-0.3527f, -0.4854f);   // V2

        glVertex2f(0.0000f, 0.6000f);   // V0
        glVertex2f(-0.3527f, -0.4854f);   // V2
        glVertex2f(0.3527f, -0.4854f);   // V3

        glVertex2f(0.0000f, 0.6000f);   // V0
        glVertex2f(0.3527f, -0.4854f);   // V3
        glVertex2f(0.5706f, 0.1854f);   // V4
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}