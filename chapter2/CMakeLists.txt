set(SRCS
        00Color
        01BasicLight
        02Materials
        03LightingMaps
        04LightCasters
        04LightCastersAttenuation
        04LightCastersSpotlight
        05Multiplelights
        StencilTesting
        )

foreach(SRC ${SRCS})
    add_executable(${SRC} src/${SRC}.cpp include/shader.h include/shader.cpp include/texture.h include/texture.cpp include/renderable_object.h include/renderable_object.cpp include/camera.h include/camera.cpp)
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
    )
endforeach()
