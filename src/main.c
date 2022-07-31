#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../includes/shader.h"

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
  // float vertices[] = {
  //    0.5f,  0.5f, 0.0f,  // top    right
  //    0.5f, -0.5f, 0.0f,  // bottom right
  //   -0.5f, -0.5f, 0.0f,  // bottom left
  //   -0.5f,  0.5f, 0.0f,  // top    left
  // };
  // uint indices[] = {
  //   0, 1, 3,             // first triangle
  //   1, 2, 3,             // second triangle
  // };
  // triangle:
  float vertices[] = {
    // positions          // colors
    -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // bottom right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   // top
  };

  /* - Vertex  Buffer Object *
   * - Vertex  Array  Object *
   * - Element Buffer Object */
  uint VBO, VAO; // , EBO;
  glGenVertexArrays(1, &VBO);
  glGenBuffers(1, &VAO);
  // glGenBuffers(1, &EBO);

  // VAO bind must happen first for storing VBO states
  glBindVertexArray(VAO);

  // VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // EBO
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  /* Vertex Attributes */
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  // Wireframe mode:
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  /* Render Loop */
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // rendering
    glClearColor(0.1f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw vertices
    shaderUse(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

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
