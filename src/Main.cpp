#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow * window);

const unsigned int WIDTH = 1000;
const unsigned int HEIGHT = 700;

int main() {
  // initialization and configuration of glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "open-model-viewer", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // load all OpenGL functions
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Faileld to initialize glad" << std::endl;
    return -1;
  }

  //set first viewport
  glViewport(0, 0, WIDTH, HEIGHT);

  // center object vertices
  float vertices[] = {
    // Coordinates         // Colors
    -0.5f,  0.5f,  0.2f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.2f,   0.0f, 1.0f, 0.0f,
     0.0f, -0.5f,  0.2f,   0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.2f,   0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.2f,   0.0f, 1.0f, 0.0f,
     0.0f, -0.5f, -0.2f,   1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.2f,   0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.2f,   0.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.2f,   0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f, -0.2f,   0.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.2f,   0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.2f,   0.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.2f,   0.0f, 0.0f, 0.0f,
     0.0f, -0.5f,  0.2f,   0.0f, 0.0f, 0.0f,
     0.0f, -0.5f, -0.2f,   0.0f, 0.0f, 0.0f,

     0.5f,  0.5f, -0.2f,   0.0f, 0.0f, 0.0f,
     0.0f, -0.5f, -0.2f,   0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.2f,   0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.2f,   0.0f, 0.0f, 0.0f,
     0.0f, -0.5f,  0.2f,   0.0f, 0.0f, 0.0f,
     0.0f, -0.5f, -0.2f,   0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f, -0.2f,   0.0f, 0.0f, 0.0f,
     0.0f, -0.5f, -0.2f,   0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.2f,   0.0f, 0.0f, 0.0f,
  };

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // color attributes
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Create Shader
  Shader myShader("vertexshader.vs", "fragmentshader.fs");

  // Enable depth
  glEnable(GL_DEPTH_TEST);

  // Set projection matrix
  myShader.use();
  glm::mat4 projection = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
  myShader.setMat4("projection", projection);

  // render
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // Background
    glClearColor(.1f, .1, .1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    myShader.use();

    // create transformations
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    model = glm::rotate(model, (float)glfwGetTime()*2.5f, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    // myShader.setMat4("model", model);
    glUniformMatrix4fv(glGetUniformLocation(myShader.progID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    myShader.setMat4("view", view);

    // render container
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 24);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
