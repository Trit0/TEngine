cmake_minimum_required(VERSION 3.26)
project(TEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Include helper functions
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompileShaders.cmake)

# Set custom Conan output folder (optional)
set(CONAN_OUTPUT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/conan_deps" CACHE STRING "Conan output folder")

# Find Conan
if(NOT EXISTS ${CONAN_OUTPUT_FOLDER}/conan_toolchain.cmake)
    find_program(CONAN_CMD conan)
    if(NOT CONAN_CMD)
        message(FATAL_ERROR "Conan not found! Please install Conan.")
    endif()

    message(STATUS "Conan dependencies not found. Installing...")
    execute_process(
            COMMAND ${CONAN_CMD} install . --output-folder=${CONAN_OUTPUT_FOLDER} --build=missing
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE CONAN_RESULT
    )
    if(NOT CONAN_RESULT EQUAL 0)
        message(FATAL_ERROR "Conan install failed")
    endif()
    message(STATUS "Conan dependencies installed successfully.")
endif()

# Include Conan toolchain
include(${CONAN_OUTPUT_FOLDER}/conan_toolchain.cmake)

# Dependencies
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
find_package(Vulkan REQUIRED)
find_package(tinyobjloader REQUIRED)
find_package(simdjson REQUIRED)
find_package(stb REQUIRED)

find_package(Vulkan COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

# Sources
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/src/*.hpp)

# Shaders
set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
file(GLOB SHADER_FILES ${SHADERS_DIR}/*.frag ${SHADERS_DIR}/*.vert)

# Engine library
add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Shader compilation
compile_shader(${PROJECT_NAME}
        FORMAT spv
        SOURCES ${SHADER_FILES}
)

file(GLOB COMPILED_SHADER_FILES ${SHADERS_DIR}/*.spv)

# Copy shader files to output
file(COPY ${COMPILED_SHADER_FILES} DESTINATION ${CMAKE_BINARY_DIR}/bin/shaders)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        glfw
        glm::glm
        Vulkan::Vulkan
        tinyobjloader::tinyobjloader
        simdjson::simdjson
        stb::stb
)
# ---------- INSTALL RULES ----------
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Export the target for use by other projects
export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Targets.cmake)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY src/ DESTINATION include
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# Install the config files
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Copy Conan files to build directory for game to use
file(COPY ${CONAN_OUTPUT_FOLDER}/conan_toolchain.cmake DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
if(EXISTS ${CONAN_OUTPUT_FOLDER}/FindPkgConfig.cmake)
    file(COPY ${CONAN_OUTPUT_FOLDER}/FindPkgConfig.cmake DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
endif()
