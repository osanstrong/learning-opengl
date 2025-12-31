/**
 * Following tutorial at https://learnopengl.com/Getting-started
 */

// #include <GL/gl.h>
#include <glad/glad.h> // include glad (which has GL/gl.h) first to avoid typedef issues
#include <GLFW/glfw3.h> 
#include <iostream>
#include <stb_image.hh>

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, char** argv) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(400, 300, "Window Test", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(); // Must be called after creating and contextualizing window    https://stackoverflow.com/questions/59960792/segmentation-fault-when-including-glad-h
    
    // Viewport adjustment
    glViewport(0, 0, 400, 300);
    auto framebuffer_size_callback = [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    };  
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Prepare triangle for the screen
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    unsigned int VBO;
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    

    // Main Render Loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Actual Rendering
        glClearColor(0.1f, 0.5f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Push to screen
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::cout << "Program launched successfully\n";
    return 0;
}