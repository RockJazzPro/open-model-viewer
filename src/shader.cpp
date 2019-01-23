#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

// create shaders
Shader::Shader(std::string vertexFilePath, std::string fragmentFilePath) {
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;

  // check if objects can throw exceptions
  vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    // open files
    vertexShaderFile.open(vertexFilePath);
    fragmentShaderFile.open(fragmentFilePath);
    std::stringstream vertexShaderStream, fragmentShaderStream;

    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();

    vertexShaderFile.close();
    fragmentShaderFile.close();

    vertexCode = vertexShaderStream.str();
    fragmentCode = fragmentShaderStream.str();
  }
  catch (std::ifstream::failure e) {
    std::cout << "shader file not found" << std::endl;
  }

  const char* vertexShaderCode = vertexCode.c_str();
  const char* fragmentShaderCode = fragmentCode.c_str();

  // compile shaders
  // vertex shader
  unsigned int vertex;
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexShaderCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");

  // fragment shader
  unsigned int fragment;
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");

  // create shader program
  progID = glCreateProgram();
  glAttachShader(progID, vertex);
  glAttachShader(progID, fragment);
  glLinkProgram(progID);
  checkCompileErrors(progID, "PROGRAM");

  // delete shader
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

// activate the shader
void Shader::use() {
  glUseProgram(progID);
}

void Shader::setUniform(const std::string &name, bool value) {
  glUniform1i(glGetUniformLocation(progID, name.c_str()), (int)value);
}

void Shader::setUniform(const std::string &name, int value) {
  glUniform1i(glGetUniformLocation(progID, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, float value) {
  glUniform1f(glGetUniformLocation(progID, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, glm::vec4 &vec) {
  glUniform4fv(glGetUniformLocation(progID, name.c_str()), 1, &vec[0]);
}

void Shader::setUniform(const std::string &name, glm::vec3 &vec) {
  glUniform3fv(glGetUniformLocation(progID, name.c_str()), 1, &vec[0]);
}

void Shader::setUniform(const std::string &name, glm::vec2 &vec) {
  glUniform2fv(glGetUniformLocation(progID, name.c_str()), 1, &vec[0]);
}

void Shader::setUniform(const std::string &name, glm::mat4 &mat) {
  glUniformMatrix4fv(glGetUniformLocation(progID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniform(const std::string &name, glm::mat3 &mat) {
  glUniformMatrix3fv(glGetUniformLocation(progID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniform(const std::string &name, glm::mat2 &mat) {
  glUniformMatrix2fv(glGetUniformLocation(progID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// check if there is a compile error
void Shader::checkCompileErrors(GLuint id, std::string type) {
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(id, 1024, NULL, infoLog);
      std::cout << "shader compilation error of type: " << type << "\n" << infoLog << std::endl;
    }
  }
  else {
    glGetProgramiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(id, 1024, NULL, infoLog);
      std::cout << "program linking error of type: " << type << "\n" << infoLog << std::endl;
    }
  }
}