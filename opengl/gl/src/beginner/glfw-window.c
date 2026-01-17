#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h> // “A loader that fetches all OpenGL functions from your driver.”
#include <GLFW/glfw3.h> // context: "A GPU state machine allocated for your program."

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

GLFWwindow *window;
static const char *window_title = "simple GLFW window";

int main(void)
{
    if (!glfwInit())
    {
        fprintf(stderr, "failed to init glfw\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if ((window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, window_title, NULL, NULL)) == NULL)
    {
        fprintf(stderr, "failed to init window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }
 
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "failed to init GLAD\n");
        return EXIT_FAILURE;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
