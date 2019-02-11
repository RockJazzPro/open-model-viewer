#ifndef mesh_h
#define mesh_h
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "shader.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texturecoord;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices,
       std::vector<Texture> &texture);
  void draw(Shader &shader);
private:
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  void setupMesh();
};

#endif
