#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void init_glfw();
auto init_glad() -> bool;
auto create_window(int width, int height) -> GLFWwindow *;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

#include "../include/shader.h"
#include "../include/texture.h"
#include "../include/renderable_object.h"
#include "stb_image.h"

auto main() -> int
{
    init_glfw();
    auto *window = create_window(800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    init_glad();
    stbi_set_flip_vertically_on_load(true);

    Shader shader(MY_SHADER_DIR + std::string("08shader_vertex.glsl"), MY_SHADER_DIR + std::string("08shader_fragment.glsl"));

    std::array<float, 32> vertices = {
            // positions       // colors         // texture coords
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   //top left
            0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f   // top right
    };
    std::array<unsigned int, 6> indices = {3,0,2,// first Triangle
                                           0,1,2};
    std::array<float, 32> vertices1 = {
            // positions       // colors         // texture coords
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   //top left
            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f   // top right
    };
    std::array<unsigned int, 6> indices1 = {3,0,2,// first Triangle
                                            0,1,2};
    RenderableObject triangle(vertices.data(), sizeof(vertices), indices.data(), sizeof(indices), MY_TEXTURE_DIR + std::string("container.jpg"));
    RenderableObject triangle1(vertices1.data(), sizeof(vertices1), indices1.data(), sizeof(indices1), MY_TEXTURE_DIR + std::string("awesomeface.jpg"));
    shader.set_int("texture1", 0);
    shader.set_int("texture2", 1);

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        // -------------------- NEW START --------------------
        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);//randians(fov),宽高比
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));//相机平移(?)：x右 y上 z屏幕外
        //         pass transformation matrices to the shader
        shader.set_mat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        shader.set_mat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        //float angle = 90.0f;
        model = glm::translate(model, {-1.0f+(float)glfwGetTime()*0.2f, 0, 0.0f});//模型平移
        //model = glm::translate(model, {(float)sin(glfwGetTime()), 0, 0.0f});
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));//按vec3的比例旋转angle
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        /*
         * 先旋转后平移会发生什么：由自转变成公转的感觉。。 为什么：平移的量也发生旋转
         * 先缩放再平移旋转：平移和旋转程度也被缩放
         * 正确的顺序：缩放 旋转 平移
         * 在代码中应该翻过来，右乘矩阵
         */
        shader.set_mat4("model", model);

        // -------------------- NEW END --------------------
        glActiveTexture(GL_TEXTURE0); //在一个shader里同时应用多个纹理，需要定义这一行
        triangle.render(shader);

        glActiveTexture(GL_TEXTURE1);

        model = glm::mat4(1.0f);
        float angle = 90.0f;
        model = glm::translate(model, {-0.8f, 0, 0.0f});//模型平移
        model = glm::rotate(model, (float)angle, glm::vec3(0.0f, 0.0f, 1.0f));//按vec3的比例旋转angle
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        shader.set_mat4("model", model);
        triangle1.render(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void init_glfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

auto init_glad() -> bool
{
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

auto create_window(int width, int height) -> GLFWwindow *
{
    GLFWwindow *window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    return window;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
