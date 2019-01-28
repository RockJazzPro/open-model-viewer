#include "camera.h"

// create Camera
Camera::Camera(float posX, float posY, float posZ, float yaw, float pitch, float speed, float sensitivity)
: yaw(yaw), pitch(pitch), speed(speed), sensitivity(sensitivity), front(glm::vec3(0.0f, 0.0f, -1.0f)){
  position = glm::vec3(posX, posY, posZ);
  worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  up = worldUp;
  updateCamera();
}

// moves the camera
void Camera::handleKeyboard(Movement direction, float deltaTime) {
  // balanced velocity without connection to the framerate
  float velocity = speed * deltaTime;
  switch (direction) {
  case FOR:
    position += front * velocity;
    break;
  case BACK:
    position -= front * velocity;
    break;
  case LEFT:
    position -= right * velocity;
    break;
  case RIGHT:
    position += right * velocity;
    break;
  }
}

// rotates the camera
void Camera::handleMouse(float xoffset, float yoffset) {
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // avoid weird camera movement
  if (pitch > 89.0f)
    pitch = 89.0f;
  else if (pitch < -89.0f)
    pitch = -89.0f;

  updateCamera();
}

// returns the view matrix
glm::mat4 Camera::getView() {
  return glm::lookAt(position, position + front, up);
}

// updates all of the camera vectors
void Camera::updateCamera() {
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front);

  right = glm::cross(front, worldUp);
  up = glm::cross(right, front);
}
