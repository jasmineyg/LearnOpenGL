#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void init_glfw();
auto init_glad() -> bool;
auto create_window(int width, int height) -> GLFWwindow *;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

/**
 * 我们用了太长时间的float vertices[]数组和unsigned int indices[]数组了，这一点都不优雅
 * 所以，这次我们打算集成出一个RenderableObject类，来管理这些数据
 */
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
    Shader shader(MY_SHADER_DIR + std::string("07shader_vertex.glsl"), MY_SHADER_DIR + std::string("07shader_fragment.glsl"));

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

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0); //在一个shader里同时应用多个纹理，需要定义这一行
        triangle.render(shader);
        shader.set_int("texture1", 0);
        glActiveTexture(GL_TEXTURE1);
        triangle1.render(shader);
        shader.set_int("texture2", 1);

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
