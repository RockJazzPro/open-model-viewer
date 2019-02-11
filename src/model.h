#ifndef model_h
#define model_h

#include "mesh.h"
#include <iostream>
#include <vector>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
  Model(const std::string &path);
  void draw(Shader &shader);
private:
  std::vector<Texture> textures_loaded;
  std::vector<Mesh> meshes;
  std::string directory;

  const aiScene* scene;
  void loadModel(const std::string &path);
  void processNode(aiNode* node);
  Mesh processMesh(aiMesh* mesh);
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
                                            aiTextureType type,
                                            std::string typeName);
};

#endif
