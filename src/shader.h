#ifndef shader
#define shader

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
  unsigned int progID;
  
  Shader(const char* vertexFilePath, const char* fragmentFilePath);
  void use();
  void setBool(const std::string &name, bool value);
  void setInt(const std::string &name, int value);
  void setFloat(const std::string &name, float value);
  void setVec4(const std::string &name, glm::vec4 &vec);
  void setVec3(const std::string &name, glm::vec3 &vec);
  void setVec2(const std::string &name, glm::vec2 &vec);
  void setMat4(const std::string &name, glm::mat4 &mat);
  void setMat3(const std::string &name, glm::mat3 &mat);
  void setMat2(const std::string &name, glm::mat2 &mat);
private:
  void checkCompileErrors(GLuint id, std::string type);
};

#endif