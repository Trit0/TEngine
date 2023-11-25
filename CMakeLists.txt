cmake_minimum_required(VERSION 3.26)
project(TEngine)

set(CMAKE_TOOLCHAIN_FILE include/conan_toolchain.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(SOURCE_FILES src/main.cpp src/window.hpp src/window.cpp src/first_app.cpp src/first_app.hpp src/pipeline.h
        src/pipeline.h
        src/pipeline.cpp
        src/device.hpp
        src/device.cpp)

find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
find_package(Vulkan REQUIRED)

message("Building with CMake version: ${CMAKE_VERSION}")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} glfw glm::glm Vulkan::Vulkan)
target_include_directories(${PROJECT_NAME} PUBLIC ${glfw3_INCLUDE_DIRS})
