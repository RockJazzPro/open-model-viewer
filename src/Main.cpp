#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow * window);

const unsigned int WIDTH = 1000;
const unsigned int HEIGHT = 700;

Camera cam(0.0f, 0.0f, 3.0f, -90.0f, 0.0f, 5.0f, 0.05f);
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

  // set mouse callback
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);

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
  myShader.setUniform("projection", projection);

  // render
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // time management
    float currentTime = (float) glfwGetTime();
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;

    // Background
    glClearColor(.1f, .1, .1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    myShader.use();

    // create transformations
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = cam.getView();

    model = glm::rotate(model, (float)glfwGetTime()*2.5f, glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(myShader.progID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    myShader.setUniform("view", view);

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

  // camera movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cam.handleKeyboard(FOR, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cam.handleKeyboard(BACK, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cam.handleKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cam.handleKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  // if mouse enters the window the first time
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  // transform mouse coordinates
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  cam.handleMouse(xoffset, yoffset);
}
