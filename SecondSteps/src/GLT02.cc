/**
 * Following tutorial at https://learnopengl.com/Getting-started
 */

// #include <GL/gl.h>
#include <glad/glad.h> // include glad (which has GL/gl.h) first to avoid typedef issues
#include <GLFW/glfw3.h> 
#include <iostream>
#include <fstream>
#include <stb_image.hh>
#define STB_IMAGE_IMPLEMENTATION


std::string file_to_string(std::string filename) {
    std::string textIter;
    std::ifstream readFile(filename);

    std::string target = "\n";
    while (std::getline (readFile, textIter)) {
        target += textIter + "\n";
    }
    std::cout << target << std::endl;
    return target;
}

/**
 * Takes the given unsigned int reference of a shader program, and generates a shader from the given files associated with that int.
 */
void generateShader(unsigned int& shader_program, std::string vert_path, std::string frag_path) {
    // Load the shader programs
    // open file for reading
    // const char *vert_source = file_to_string(vert_path).c_str();
    const char *vert_source = "#version 330 core\n"
                            "layout (location = 0) in vec3 aPos;\n"
                            "void main()\n"
                            "{\n"
                            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                            "}\0";
    // const char *frag_source = file_to_string(frag_path).c_str();
    const char *frag_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";
    // Compile shaders
    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_source, NULL);
    glCompileShader(vert_shader);
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_source, NULL);
    glCompileShader(frag_shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vert_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cout << "Original source: \n" << (vert_source) << std::endl;
        printf("%s\n", vert_source);
    }
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Generate shader program at this address
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);

    // Tell OpenGL to use this new shader instead
    glUseProgram(shader_program);

    // Delete shader objects once no longer needed
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}


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
    
    // Initialize shader and tell it how to interpret our vertex data
    unsigned int shader;
    generateShader(shader, "./src/shad/bland_vert.glsl", "./src/shad/bland_frag.glsl");

    int success;
    char infoLog[512];
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success) {
        std::cout << "Program not compiled succesfully :(" << std::endl;
        glGetProgramInfoLog(shader, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
    }

    // Create VAO wrapper to store configuration
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    std::cout << glGetError() << std::endl;
    // Bind it
    glBindVertexArray(VAO);
    std::cout << glGetError() << std::endl;
    // Copy vertices array to VBO for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << glGetError() << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    std::cout << glGetError() << std::endl;
    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    std::cout << glGetError() << std::endl;
    glEnableVertexAttribArray(0);  
    std::cout << glGetError() << std::endl;

    // Main Render Loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Actual Rendering
        glClearColor(0.1f, 0.5f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Push to screen
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::cout << "Program launched successfully\n";
    return 0;
}