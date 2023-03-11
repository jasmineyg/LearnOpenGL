#ifndef LEARNOPENGL_MESH_H
#define LEARNOPENGL_MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include "shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct TextureStruct {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    /*  网格数据  */
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureStruct> textures;
    unsigned int VAO;
    /*  函数  */
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureStruct> textures);
    void Draw(const Shader &shader);
private:
    /*  渲染数据  */
    unsigned int VBO, EBO;
    /*  函数  */
    void setupMesh();
};

#endif //LEARNOPENGL_MESH_H
