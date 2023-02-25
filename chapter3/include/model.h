#ifndef LEARNOPENGL_MODEL_H
#define LEARNOPENGL_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "Mesh.h"

class Model
{
public:
    /*  函数   */
    Model(char *path)
    {
        loadModel(path);
    }
    void Draw(Shader shader);
private:
    /*  模型数据  */
    std::vector<Mesh> meshes;
    std::string directory;
    /*  函数   */
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         std::string typeName);
};

#endif //LEARNOPENGL_MODEL_H
