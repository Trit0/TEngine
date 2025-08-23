cmake_minimum_required(VERSION 3.26)
project(SampleGame VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

## Check if we're building as part of a larger project or standalone
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # Standalone build - find TEngine as an external package
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../TEngine/build")

    # Include the Conan toolchain from engine's conan_deps folder
    set(ENGINE_CONAN_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../TEngine/conan_deps")
    if(EXISTS "${ENGINE_CONAN_FOLDER}/conan_toolchain.cmake")
        include("${ENGINE_CONAN_FOLDER}/conan_toolchain.cmake")
    endif()

    # Find the TEngine package
     find_package(TEngine REQUIRED)
else()
    # Standalone build - find TEngine as an external package
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../TEngine")

    # Include the Conan toolchain from engine's conan_deps folder
    set(ENGINE_CONAN_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../TEngine")
    if(EXISTS "${ENGINE_CONAN_FOLDER}/conan_toolchain.cmake")
        include("${ENGINE_CONAN_FOLDER}/conan_toolchain.cmake")
    endif()
endif()

# --- Game sources ---
file(GLOB_RECURSE SampleGame_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SampleGame_SOURCES})

# Set include directories
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link to the engine (transitive include dirs/defs come from the engine target)
target_link_libraries(${PROJECT_NAME} PRIVATE TEngine)

# (Optional) Where to put the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Copy any necessary resources/assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin)

# (Optional) strict warnings for your game code
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
