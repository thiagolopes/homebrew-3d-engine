cmake_minimum_required(VERSION 3.12)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

# Find OpenGL
find_package(OpenGL REQUIRED)

add_subdirectory(glew/build/cmake)
include_directories(glew/include)


add_subdirectory(glfw)
include_directories(glfw/include)

set(SOURCES
    src/main.cpp
)

add_executable(OpenGLProject ${SOURCES})

target_link_libraries(OpenGLProject
    PRIVATE
        OpenGL::GL
        glfw
        glew
)
