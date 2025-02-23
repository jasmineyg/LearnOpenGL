#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

void init_glfw();
auto init_glad() -> bool;
auto create_window(int width, int height) -> GLFWwindow *;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
auto compile_shader(const char *vertex_shader_source, const char *fragment_shader_source) -> unsigned int;
auto pass_geometry_data_to_GPU(float vertices_array[], int vertices_array_size, unsigned int indices_array[], int indices_array_size) -> unsigned int;
auto read_file(const std::string &file_path) -> std::string;

/**
 * 我们给每个顶点都设置上颜色，这样我们就可以在片段着色器里面使用这个颜色了。
 */
float vertices[] = {
        // positions                    // colors
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // top

};
unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
};

/**
 * 这里因为顶点的数据变了，所以我们需要重新写一下把顶点数据传到顶点着色器的函数。
 */
auto pass_geometry_data_to_GPU(float vertices_array[], int vertices_array_size, unsigned int indices_array[], int indices_array_size) -> unsigned int
{
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_array_size, vertices_array, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_array_size, indices_array, GL_STATIC_DRAW);

    // -------------------- NEW START --------------------
    /**
     * 我们需要重新指定内存取址的偏移量
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float))); // 3 * sizeof(float) 是因为前面有3个float
    glEnableVertexAttribArray(1);
    // -------------------- NEW END --------------------

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

auto main() -> int
{
    init_glfw();
    auto *window = create_window(800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    init_glad();

    std::string vertex_source_code = read_file(MY_SHADER_DIR + std::string("05shader_vertex.glsl"));
    std::string fragment_source_code = read_file(MY_SHADER_DIR + std::string("05shader_fragment.glsl"));
    unsigned int shader_program_id = compile_shader(vertex_source_code.c_str(), fragment_source_code.c_str());
    unsigned int triangle_VAO = pass_geometry_data_to_GPU(vertices, sizeof(vertices), indices, sizeof(indices));

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program_id);
        glBindVertexArray(triangle_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

auto compile_shader(const char *vertex_shader_source, const char *fragment_shader_source) -> unsigned int
{
    // 编译Vertex Shader
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader_id);

    // 编译Fragment Shader
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader_id);

    // 创建Shader Program
    unsigned int shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vertex_shader_id);
    glAttachShader(shader_program_id, fragment_shader_id);
    glLinkProgram(shader_program_id);

    /**
     * 可选项：检查上面三个编译是否成功
     */
    int success;
    char infoLog[512];
    // 检查Vertex Shader是否编译成功
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader_id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED" << std::endl;
    }
    // 检查Fragment Shader是否编译成功
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader_id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED" << std::endl;
    }
    // 检查Shader Program是否链接成功
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program_id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::LINK_FAILED" << std::endl;
    }

    /**
     * 为了节省显存，我们可以在这里删除Shader，因为它们已经被链接到了Shader Program中了。
     */
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return shader_program_id;
}

auto read_file(const std::string &file_path) -> std::string
{
    std::ifstream file_stream(file_path);
    std::string file_content((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());
    return file_content;
}