#include "model.h"
#include <assimp/Importer.hpp>
#include <stb_image/stb_image.h>

unsigned int TextureFromFile(const char* path, const std::string &directory);

// initialize model
Model::Model(const std::string &path) {
  loadModel(path);
}

void Model::draw(Shader &shader) {
  for (unsigned int i = 0; i < meshes.size(); i++)
    meshes[i].draw(shader);
}

void Model::loadModel(const std::string &path) {
  Assimp::Importer importer;
  // create scene
  scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  // check for scene errors
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cout << "assimp error: " << importer.GetErrorString() << std::endl;
    return;
  }
  directory = path.substr(0, path.find_last_of('/'));
  // process the root node
  processNode(scene->mRootNode);
}

void Model::processNode(aiNode* node) {

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh));
  }

  // recursive iteration over all nodes
  for (unsigned int i = 0; i < node->mNumChildren; i++)
    processNode(node->mChildren[i]);
}

Mesh Model::processMesh(aiMesh* mesh) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  // handle vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;

    // get positions
    glm::vec3 position;
    position.x = mesh->mVertices[i].x;
    position.y = mesh->mVertices[i].y;
    position.z = mesh->mVertices[i].z;

    // get normals
    glm::vec3 normal;
    normal.x = mesh->mNormals[i].x;
    normal.y = mesh->mNormals[i].y;
    normal.z = mesh->mNormals[i].z;

    // get texture coords
    glm::vec2 texturecoord;
    if (mesh->mTextureCoords[0]) {
      texturecoord.x = mesh->mTextureCoords[0][i].x;
      texturecoord.y = mesh->mTextureCoords[0][i].y;
    }
    else
      texturecoord = glm::vec2(0.0f, 0.0f);

    vertex.position = position;
    vertex.normal = normal;
    vertex.texturecoord = texturecoord;

    vertices.push_back(vertex);
  }

  // handle indices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  // handle textures
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  
  // build the textures
  std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
  std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
  std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
  // insert the textures
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

  return Mesh(vertices, indices, textures);
}

// load the textures
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
  std::vector<Texture> textures;
  // check if there is a texture which is already loaded
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString path;
    mat->GetTexture(type, i, &path);
    bool alreadyLoaded = false;
    // check if any texture is already loaded
    for (unsigned int j = 0; j < textures_loaded.size(); j++)
      if (std::strcmp(textures_loaded[j].path.data(), path.C_Str()) == 0) {
        textures.push_back(textures_loaded[j]);
        alreadyLoaded = true;
        break;
      }
    if (!alreadyLoaded) {
      Texture texture;
      texture.id = TextureFromFile(path.C_Str(), this->directory);
      texture.type = typeName;
      texture.path = path.C_Str();
      textures.push_back(texture);
      textures_loaded.push_back(texture);
    }
  }
  return textures;
}

// bind texture
unsigned int TextureFromFile(const char* path, const std::string &directory) {
  std::string filename = std::string(path);
  filename = directory + '/' + filename;
  
  unsigned int textureID;
  glGenTextures(1, &textureID);
  
  int width;
  int height;
  int formatComp;
  unsigned char* data = stbi_load(filename.c_str(), &width, &height, &formatComp, 0);
  if (data) {
    GLenum format;
    switch (formatComp) {
    case 1:
      format = GL_RED; break;
    case 3:
      format = GL_RGB; break;
    case 4:
      format = GL_RGBA; break;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else
    std::cout << "Couldn't load texture: " << path << std::endl;

  // release the data
  stbi_image_free(data);
  return textureID;
}