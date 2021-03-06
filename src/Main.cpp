#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include "model.h"
#include <assimp/Importer.hpp>
#include <stb_image/stb_image_write.h>;
#include <tinyfiledialogs.h>
#include <algorithm>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void shortcut_callback(GLFWwindow* window, int key, int scancode, int action,
                       int mods);
void drop_callback(GLFWwindow* window, int count, const char** paths);
void processMovement(GLFWwindow * window);
void exportImage(const std::string &name);

unsigned int WIDTH = 1000;
unsigned int HEIGHT = 700;

Camera cam(0.0f, 0.0f, 3.0f, -90.0f, 0.0f, 5.0f, 0.05f);
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool wireframeMode = false;

Model* mainModel;

int main() {
  // initialization and configuration of glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "open-model-viewer",
      NULL, NULL);
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

  // set key callback
  glfwSetKeyCallback(window, shortcut_callback);

  // set drop callback
  glfwSetDropCallback(window, drop_callback);

  // load all OpenGL functions
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize glad" << std::endl;
    return -1;
  }

  //set first viewport
  glViewport(0, 0, WIDTH, HEIGHT);

  // Create Shader
  Shader myShader("vertexshader.vs", "fragmentshader.fs");

  // Create Model
  mainModel = new Model("res/nanosuit/nanosuit.obj");

  // Enable depth
  glEnable(GL_DEPTH_TEST);

  // render
  while (!glfwWindowShouldClose(window)) {
    processMovement(window);

    // time management
    float currentTime = (float) glfwGetTime();
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;

    // Background
    glClearColor(.1f, .1, .1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set projection matrix
    myShader.use();
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f),
        (float)WIDTH / (float)HEIGHT, // aspect ratio
        0.1f, 100.0f);
    myShader.setUniform("projection", projection);

    // create transformations
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = cam.getView();
    myShader.setUniform("model", model);
    myShader.setUniform("view", view);
    

    mainModel->draw(myShader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void shortcut_callback(GLFWwindow* window, int key, int scancode, int action,
                       int mods) {
  // exit application if escape is pressed
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // enter wireframe mode if 'm' is pressed
  if (key == GLFW_KEY_M && action == GLFW_PRESS) {
    wireframeMode = !wireframeMode;
    glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
  }

  // import new object if ctrl + i is pressed
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
      glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS &&
      action == GLFW_PRESS) {
    const char* filterPatterns[1] = { "*.obj" };
    const char* filename = tinyfd_openFileDialog(
        "Choose the model file you want to load",
        "", 1, filterPatterns, NULL, 0);
    if (filename != nullptr) {
      std::string newfile = filename;
      std::replace(newfile.begin(), newfile.end(), '\\', '/');
      delete mainModel;
      mainModel = new Model(newfile);
    }
    else
      std::cout << "Please enter a valid obj file!" << std::endl;
  }

  // export current frame as png
  if (key == GLFW_KEY_E && action == GLFW_PRESS) {
    const char* filterPatterns[1] = { "*.png" };
    const char* filename = tinyfd_saveFileDialog("Choose a location",
        "open-model-viewer.png", 1, filterPatterns, NULL);
    if (filename != nullptr)
      exportImage(filename);
    else
      std::cout << "Please enter a valid file name!" << std::endl;
  }
}

void processMovement(GLFWwindow *window) {
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
  WIDTH = width;
  HEIGHT = height;
}

void drop_callback(GLFWwindow* window, int count, const char** paths) {
  // check dragged files
  if (paths[count - 1] != nullptr) {
    std::string newfile = paths[count-1];
    std::replace(newfile.begin(), newfile.end(), '\\', '/');
    delete mainModel;
    mainModel = new Model(newfile);
  }
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

void exportImage(const std::string &name) {
  // Buffer object with frame informations
  GLubyte* buffer = new GLubyte[3 * WIDTH * HEIGHT];
  glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);

  stbi_flip_vertically_on_write(1);
  stbi_write_png(name.c_str(), WIDTH, HEIGHT, 3, buffer, WIDTH * 3);

  // Delete the buffer data
  delete[] buffer;
}
