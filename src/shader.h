#ifndef shader
#define shader

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
  unsigned int progID;
  
  Shader(std::string vertexFilePath, std::string fragmentFilePath);
  void use();
  void setUniform(const std::string &name, bool value);
  void setUniform(const std::string &name, int value);
  void setUniform(const std::string &name, float value);
  void setUniform(const std::string &name, glm::vec4 &vec);
  void setUniform(const std::string &name, glm::vec3 &vec);
  void setUniform(const std::string &name, glm::vec2 &vec);
  void setUniform(const std::string &name, glm::mat4 &mat);
  void setUniform(const std::string &name, glm::mat3 &mat);
  void setUniform(const std::string &name, glm::mat2 &mat);
private:
  void checkCompileErrors(GLuint id, std::string type);
};

#endif