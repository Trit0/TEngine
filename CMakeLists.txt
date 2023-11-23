cmake_minimum_required(VERSION 3.27)
project(TEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD ${CONAN_CMAKE_CXX_STANDARD})
set(SOURCE_FILES src/main.cpp src/window.hpp src/window.cpp
        src/first_app.cpp
        src/first_app.hpp)

find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
find_package(Vulkan REQUIRED)

message("Building with CMake version: ${CMAKE_VERSION}")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} glfw glm::glm Vulkan::Vulkan)
