/**
 * Following tutorial at https://learnopengl.com/Getting-started
 */

// #include <GL/gl.h>
#include <glad/glad.h> // include glad (which has GL/gl.h) first to avoid typedef issues
#include <GLFW/glfw3.h> 
#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.hh>

// Timer 
#include <chrono>
using time_point = std::chrono::steady_clock::time_point;
#define clock std::chrono::steady_clock

// OpenGL error codes that were undefined for some reason
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504

// Window size 
int win_width = 400;
int win_height = 300;

// Total elapsed time
float elapsed_time = 0;



std::string file_to_string(std::string filename) {
    std::string textIter;
    std::ifstream readFile(filename);

    std::string target = "\n";
    while (std::getline (readFile, textIter)) {
        target += textIter + "\n";
    }
    return target;
}

/***
 * 
 */
GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

/**
 * Takes the given unsigned int reference of a shader program, and generates a shader from the given files associated with that int.
 */
void generateShader(unsigned int& shader_program, std::string vert_path, std::string frag_path) {
    // Load the shader programs
    // open file for reading
    // const char *vert_source = file_to_string(vert_path).c_str();
//     const char *vert_source = "#version 330 core\n"
//                             "layout (location = 0) in vec3 aPos;\n"
//                             "void main()\n"
//                             "{\n"
//                             "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//                             "}\0";
//     // const char *frag_source = file_to_string(frag_path).c_str();
//     const char *frag_source = "#version 330 core\n"
// "out vec4 FragColor;\n"
// "void main()\n"
// "{\n"
// "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
// "}\0";
    std::string vert_string = file_to_string(vert_path);
    std::string frag_string = file_to_string(frag_path);
    const char* vert_source = vert_string.c_str();
    const char* frag_source = frag_string.c_str();

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
    
    // Load the image we want to use
    int width, height, num_channels;
    stbi_set_flip_vertically_on_load(true);
    
    unsigned int texture1, texture2;
    // Texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned char *data = stbi_load("../data/datHair.png", &width, &height, &num_channels, 0);
    if (!data) std::cout << "Failed to load texture!" << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,  data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // Texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    data = stbi_load("../data/Spring2024_credits.png", &width, &height, &num_channels, 0);
    if (!data) std::cout << "Failed to load texture 2!" << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,  data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);



    // Viewport adjustment
    glViewport(0, 0, win_width, win_height);
    auto framebuffer_size_callback = [](GLFWwindow* window, int width, int height) {
        win_width = width;
        win_height = height;
        glViewport(0, 0, width, height);
    };  
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Prepare triangles of quad for the screen
    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f,  0.5f, 0.0f,
    //     -1.0f, 0.5f, 0.0f
    // };

    // unsigned int indices[] = {
    //     0, 1, 2, // First triangle
    //     2, 3, 0  // Second triangle
    // };

    // Rather than triangle, now prepare big rectangle
    float u, v, hw, hh;
    u = 0.5f;
    v = 0.5f;
    hw = 0.05f;
    hh = 0.05f;
    float vertices[] = {
        // positions          // colors           // texture coords
        0.25f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   u+hw, v+hh,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   u+hw, v-hh,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   u-hw, v-hh,   // bottom left
        -0.75f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   u-hw, v+hh    // top left 
    };
    
    unsigned int indices[] {
        0, 1, 2,
        2, 3, 0
    };
    
    // Original two-texture shader
    unsigned int shader;
    generateShader(shader, "./src/shad/tex_vert.vert", "./src/shad/tex_frag.frag");
    
    // Shader for framebuffer display
    unsigned int screen_shader;
    generateShader(screen_shader, "./src/shad/screen_vert.vert", "./src/shad/screen_frag.frag");

    int success;
    char infoLog[512];
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success) {
        std::cout << "Program not compiled succesfully :(" << std::endl;
        glGetProgramInfoLog(shader, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
    }

    // Create VAO wrapper to store configuration, VBO for vertices, and EBO for indices
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glCheckError();
    // Bind it
    glBindVertexArray(VAO);
    glCheckError();
    // Copy vertices array to VBO for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glCheckError();
    // Copy indices array to EBO for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glCheckError();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glCheckError();
    // Set vertex attribute pointers
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    // Color
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Tex coord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // Set texture bind uniforms
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "u_texture1"), 0);
    glUniform1i(glGetUniformLocation(shader, "u_texture2"), 1);

    // Vertices and array objects for screenspace stuff
    float quad_verts[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_verts), &quad_verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    
    // Configure framebuffer/renderbuffer to store output intermediate
    unsigned int fbo;
    glCheckError();
    glGenFramebuffers(1, &fbo);
    glCheckError();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // Generate texture for frame buffer
    unsigned int tex_colorBuffer;
    glCheckError();
    glGenTextures(1, &tex_colorBuffer);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, tex_colorBuffer);
    glCheckError();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, win_width, win_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // Null because we fill it with the fbo output
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
    glCheckError();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_colorBuffer, 0);
    glCheckError();
    // Render buffer object for the other stuff bc we don't actually need to use it
    unsigned int rbo;
    glCheckError();
    glGenRenderbuffers(1, &rbo);
    glCheckError();
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glCheckError();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, win_width, win_height);
    glCheckError();
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glCheckError();
    // Attach rbo to depth and stencil attachments
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glCheckError();
    // See if we set it up correctly
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
    glCheckError();

    time_point start = clock::now();
    time_point last = start;

    // Main Render Loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Bind framebuffer before rendering scene as normal
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        // glEnable(GL_DEPTH_TEST); // Enable for original scene, though our mini test doesn't use it
            // Actual Rendering
            glClearColor(0.1f, 0.5f, 0.6f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We don't use the depth buffer here but for completion

            glUseProgram(shader);
            glCheckError();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
            glCheckError();


            glBindVertexArray(VAO);
            glCheckError();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glCheckError();
            glBindVertexArray(0);
        
        //Then bind back to default framebuffer and draw quad using that fbo's texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // no depth testing
        //Clear buffers
        float bright = 0.8f;
        glClearColor(bright, bright, bright, 1.0f); // Different color to make it clear
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(screen_shader);
        float shift = std::sin(elapsed_time*2) * 0.5f;
        float shift_y = std::cos(elapsed_time*2) * 0.5f;
        glUniform1f(glGetUniformLocation(screen_shader, "u_shift"), shift);
        glUniform1f(glGetUniformLocation(screen_shader, "u_shiftY"), shift_y);

        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_colorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Push to screen
        glfwSwapBuffers(window);
        glfwPollEvents();

        time_point next = clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::microseconds>(next - last).count() / 1000.0f;

        elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(next - start).count() / 1000000.0f;
        // std::cout << "Time elapsed (s): " << elapsed_time << std::endl;
        last = next;


    }

    glfwTerminate();
    std::cout << "Program launched successfully\n";
    return 0;
}