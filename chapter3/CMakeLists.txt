set(SRCS
        01AddModel
        02DepthTesting
        03StencilTesting
        04Blending
        04Blending2
        )

foreach(SRC ${SRCS})
    add_executable(${SRC} src/${SRC}.cpp include/shader.h include/shader.cpp include/texture.h include/texture.cpp include/renderable_object.h include/renderable_object.cpp include/camera.h include/camera.cpp include/Mesh.h include/Mesh.cpp include/model.h include/model.cpp include/object.h include/object.cpp)
    set_target_properties(${SRC} PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
    target_include_directories(
            ${SRC}
            PRIVATE
            src
            ../deps/stb
    )
    target_link_libraries(
            ${SRC}
            PRIVATE
            assimp::assimp
            glm::glm
            glfw
            glad
    )
    target_compile_definitions(
            ${SRC}
            PRIVATE
            MY_SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shader/"
            MY_TEXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/texture/"
            MY_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/"
            MY_MODEL_DIR="${CMAKE_CURRENT_SOURCE_DIR}/model/"
    )
endforeach()