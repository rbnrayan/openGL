#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned int uint;

const uint WINDOW_WIDTH  = 800;
const uint WINDOW_HEIGHT = 600;

/* Shaders */
const char *vertexShaderSource =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "layout (location = 1) in vec3 aColor;\n"
  "out vec3 ourColor;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "  ourColor = aColor;\n"
  "}\n\0";

const char *fragmentShaderSource =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(ourColor, 1.0f);\n"
  "}\n\0";

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

  /* Shaders compilation errors check variables */
  int success;
  char infoLog[512];

  /* Vertex Shader */
  uint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  /* Shader compilation errors check */
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
  }

  /* Fragment Shader */
  uint fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  /* Shader compilation errors check */
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
  }

  /* Shader Program */
  uint shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  /* Shader program errors check */
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    fprintf(stderr, "ERROR::SHADER::PROGRAM::LINK_FAILED\n%s\n", infoLog);
  }

  /* Shaders deletion */
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

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
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

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
