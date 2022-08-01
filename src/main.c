/* Includes */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../includes/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../includes/stb_image.h"

typedef unsigned int uint;

const uint WINDOW_WIDTH  = 800;
const uint WINDOW_HEIGHT = 600;

void processInput(GLFWwindow *);
void framebuffer_size_callback(GLFWwindow *, int, int);

int main(void)
{
  /* Setting up GLFW for getting the proper context for OpenGL 
                  which is OS specific */
  if (glfwInit() == GL_FALSE)
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    exit(1);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    fprintf(stderr, "Failed to create GLFW window\n");
    exit(1);
  }
  glfwMakeContextCurrent(window);

  /* Loads OpenGL function pointers (OS specific too) */
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
  {
    fprintf(stderr, "Failed to initialize GLAD\n");
    exit(1);
  }

  /* Set the OpenGL viewport to match the window width and height */
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  /* Shaders */
  Shader shader_program =
      shaderNew("shaders/vertex.glsl", "shaders/fragment.glsl");

  /* Vertices data */
  // rectangle with EBO:
  float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top    right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top    left
  };
  uint indices[] = {
    0, 1, 3,              // first triangle
    1, 2, 3,              // second triangle
  };
  // triangle:
  // float vertices[] = {
  //   // positions          // colors
  //   -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // bottom right
  //    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   // bottom left
  //    0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   // top
  // };

  /* - Vertex  Buffer Object *
   * - Vertex  Array  Object *
   * - Element Buffer Object */
  uint VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // VAO bind must happen first for storing VBO states
  glBindVertexArray(VAO);

  // VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  /* Vertex Attributes */
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coords attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  /* Textures */
  uint texture1, texture2;

  // texture1
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // texture wrapping options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  // flip image
  stbi_set_flip_vertically_on_load(TRUE);
  unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    fprintf(stderr, "Failed to load texture (1)\n");
  }
  stbi_image_free(data);

  // texture 2
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  // texture wrapping options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    fprintf(stderr, "Failed to load texture (2)\n");
  }
  stbi_image_free(data);

  // Wireframe mode:
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  shaderUse(shader_program);
  shaderSetInt(shader_program, "texture1", 0);
  shaderSetInt(shader_program, "texture2", 1);

  /* Render Loop */
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // rendering
    glClearColor(0.1f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // draw vertices
    shaderUse(shader_program);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}
