#ifndef LEARNOPENGL_MODEL_H
#define LEARNOPENGL_MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "Mesh.h"

class Model
{
public:
    std::string directory;
    bool gammaCorrection;

    Model(std::string const &path, bool gamma = false);
    void Draw(Shader &shader);
private:
    std::vector<TextureStruct> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh>    meshes;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    //unsigned int TextureFromFile(const char *path, const std::string &directory);
    std::vector<TextureStruct> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                              std::string typeName);
};

#endif //LEARNOPENGL_MODEL_H
