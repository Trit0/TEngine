cmake_minimum_required(VERSION 3.26)
project(TEngine)

set(CMAKE_TOOLCHAIN_FILE include/conan_toolchain.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(SOURCE_FILES src/main.cpp src/window.hpp src/window.cpp src/first_app.cpp src/first_app.hpp src/pipeline.hpp
        src/pipeline.hpp
        src/pipeline.cpp
        src/device.hpp
        src/device.cpp
        src/swap_chain.hpp
        src/swap_chain.cpp
        src/model.cpp
        src/model.hpp
        src/game_object.hpp
        src/renderer.cpp
        src/renderer.hpp
        src/simple_render_system.cpp
        src/simple_render_system.hpp
        src/camera.cpp
        src/camera.hpp
        src/keyboard_movement_controller.cpp
        src/keyboard_movement_controller.hpp)

set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders)

find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
find_package(Vulkan REQUIRED)

find_package(Vulkan COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

function(compile_shader target)
    cmake_parse_arguments(PARSE_ARGV 1 arg "" "ENV;FORMAT" "SOURCES")
    foreach(source ${arg_SOURCES})
        add_custom_command(
                TARGET ${PROJECT_NAME}
                COMMAND
                ${glslc_executable}
                ${source}
                -o ${source}.${arg_FORMAT}
        )
        target_sources(${target} PRIVATE ${source}.${arg_FORMAT})
    endforeach()
endfunction()

message("Building with CMake version: ${CMAKE_VERSION}")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

compile_shader(${PROJECT_NAME}
        FORMAT spv
        SOURCES
        ${SHADERS_DIR}/simple_shader.vert
        ${SHADERS_DIR}/simple_shader.frag
)

target_link_libraries(${PROJECT_NAME} glfw glm::glm Vulkan::Vulkan)
target_include_directories(${PROJECT_NAME} PUBLIC ${glfw3_INCLUDE_DIRS} PUBLIC ${glm_INCLUDE_DIRS})
