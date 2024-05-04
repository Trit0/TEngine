cmake_minimum_required(VERSION 3.26)
project(TEngine)

set(CMAKE_TOOLCHAIN_FILE include/conan_toolchain.cmake)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB SHADER_FILES ${SHADERS_DIR}/*.frag ${SHADERS_DIR}/*.vert)

find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
find_package(Vulkan REQUIRED)
find_package(tinyobjloader REQUIRED)

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

add_executable(${PROJECT_NAME} ${SOURCES})

compile_shader(${PROJECT_NAME}
        FORMAT spv
        SOURCES
        ${SHADER_FILES}
)

target_link_libraries(${PROJECT_NAME} glfw glm::glm Vulkan::Vulkan tinyobjloader::tinyobjloader)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS} ${tinyobjloader_INCLUDE_DIRS})