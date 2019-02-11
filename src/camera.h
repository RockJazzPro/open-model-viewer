#ifndef camera_h
#define camera_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum Movement {
  FOR,
  BACK,
  LEFT,
  RIGHT
};

class Camera {
public:
  Camera(float posX, float posY, float posZ, float yaw, float pitch,
         float speed, float sensitivity);
  void handleKeyboard(Movement direction, float deltaTime);
  void handleMouse(float xoffset, float yoffset);
  glm::mat4 getView();
private:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 worldUp;
  glm::vec3 right;
  float yaw;
  float pitch;
  float speed;
  float sensitivity;

  void updateCamera();
};
#endif
