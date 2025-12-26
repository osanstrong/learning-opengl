/*
 * Test OpenGL C++ setup
 */

#include <GL/glut.h>
#include <GLES3/gl3.h>
#include <iostream>
#include <fstream>
// #include <GL/freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// #include <GLFW/glfw3.h>
// #include <GL/glext.h>


std::string file_to_string(std::string filename) {
    std::string textIter;
    std::ifstream readFile(filename);

    std::string target = "";
    while (std::getline (readFile, textIter)) {
        target += textIter + "\n";
    }
    return target;
  // // open file for reading
  //   std::ifstream istrm(filename, std::ios::binary);
  //   if (!istrm.is_open()){
  //       std::cout << "failed to open " << filename << '\n';
  //       return "";
  //   }
  //   else
  //   {
  //       double d;
  //       istrm.read(reinterpret_cast<char*>(&d), sizeof d); // binary input
  //       int n;
  //       std::string s;
  //       if (istrm >> n >> s){                               // text input
  //           std::cout << "read back from file: " << d << ' ' << n << ' ' << s << '\n';
  //           return s;
  //       } else {
  //         return "";  
  //       }
  //   }
  //   return "";
}

/**
 * Handler for window repaint/refresh event
 */
void display() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear black & opaque 
  glClear(GL_COLOR_BUFFER_BIT);

  // 1 x 1 Red Square centered at origin
  glBegin(GL_QUADS);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex2f(-0.5f, -0.5f);
      glVertex2f( 0.5f, -0.5f);
      glVertex2f( 0.5f,  0.5f);
      glVertex2f(-0.5f,  0.5f);
  glEnd();

  glFlush(); // Push to screen

  // Make image vertices and use those actually here we go
  float vertices[] = {
    // positions          // colors           // texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);  
}

/**
 * Main function / entry point for program.
 * GLUT runs as a console application using main() as a starting point.
 */
int main(int argc, char** argv) {

   // Load the shader programs
   // open file for reading
   std::string vert_source = file_to_string("shad/test_vert.glsl");
   std::string frag_source = file_to_string("shad/test_frag.glsl");
  //  std::cout << "Complete vert source: \n----\n" << vert_source << "\n";
   
   unsigned int texture;
   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);
   // wrapping / filtering options 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Load the texture (Referencing https://learnopengl.com/Getting-started/Textures)
   int width, height, nrChannels; 
   unsigned char *data = stbi_load("datHair.png", &width, &height, &nrChannels, 0);
  
   if (data) {
     // Generate the texture and mipmaps
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
     glGenerateMipmap(GL_TEXTURE_2D);
   } else {
     std::cout << "Failed to load texture" << std::endl;
   }
   // Free original image memory
   stbi_image_free(data);

   glutInit(&argc, argv);
   glutCreateWindow("OpenGL Setup Test");
   glutInitWindowSize(320, 320);
   glutInitWindowPosition(50, 50); //Starting top-left corner of window
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}