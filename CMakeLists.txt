cmake_minimum_required(VERSION 3.26)
project(TEngine)

set(CMAKE_TOOLCHAIN_FILE include/conan_toolchain.cmake)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
find_package(Vulkan REQUIRED)
find_package(tinyobjloader REQUIRED)
find_package(simdjson REQUIRED)

set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
file(GLOB SHADER_FILES ${SHADERS_DIR}/*.frag ${SHADERS_DIR}/*.vert)

find_package(Vulkan COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

function(compile_shader target)
    cmake_parse_arguments(PARSE_ARGV 1 arg "" "ENV;FORMAT" "SOURCES")

    file(GLOB_RECURSE compiled_shaders "${SHADERS_DIR}/*.${arg_FORMAT}")

    foreach(compiled_shader ${compiled_shaders})
        # Get the original source file path by removing the format extension
        string(REGEX REPLACE "\\.${arg_FORMAT}$" "" source_file ${compiled_shader})

        # Check if the corresponding source file exists
        if (NOT EXISTS ${source_file})
            message(STATUS "Deleting orphaned shader: ${compiled_shader}")
            file(REMOVE ${compiled_shader})
        endif()
    endforeach()

    foreach(source ${arg_SOURCES})
        get_filename_component(filename ${source} NAME)
        get_filename_component(filepath ${source} DIRECTORY)
        set(output_file ${filepath}/${filename}.${arg_FORMAT})

        string(TOUPPER ${arg_FORMAT} FORMAT_UPPER)

        add_custom_command(
                OUTPUT ${output_file}
                COMMAND ${glslc_executable} ${source} -o ${output_file}
                DEPENDS ${source}
                COMMENT "Compiling ${FORMAT_UPPER} shader ${filename}.${arg_FORMAT}"
        )

        target_sources(${target} PRIVATE ${output_file})
    endforeach()
endfunction()

message("Building with CMake version: ${CMAKE_VERSION}")

add_executable(${PROJECT_NAME} ${SOURCES})

compile_shader(${PROJECT_NAME}
        FORMAT spv
        SOURCES ${SHADER_FILES}
)

target_link_libraries(${PROJECT_NAME} glfw glm::glm Vulkan::Vulkan tinyobjloader::tinyobjloader simdjson::simdjson)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${glfw3_INCLUDE_DIRS} ${glm_INCLUDE_DIRS} ${tinyobjloader_INCLUDE_DIRS})