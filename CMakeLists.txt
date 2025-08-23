cmake_minimum_required(VERSION 3.26)
project(GameProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Add subdirectories
add_subdirectory(TEngine)
add_subdirectory(SampleGame)

add_dependencies(SampleGame TEngine)