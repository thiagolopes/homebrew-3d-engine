cmake_minimum_required(VERSION 3.12)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

# Find OpenGL
find_package(OpenGL REQUIRED)

add_subdirectory(glew/build/cmake)
include_directories(glew/include)

add_subdirectory(glfw)
include_directories(glfw/include)

set(SOURCES
  src/main.cpp
  src/renderer.cpp
  src/buffers.cpp
  src/shaders.cpp
  src/textures.cpp
)
set(IMGUI_SOURCES
  src/vendor/imgui/imgui.cpp
  src/vendor/imgui/imgui_demo.cpp
  src/vendor/imgui/imgui_draw.cpp
  src/vendor/imgui/imgui_impl_glfw.cpp
  src/vendor/imgui/imgui_impl_opengl3.cpp
  src/vendor/imgui/imgui_tables.cpp
  src/vendor/imgui/imgui_widgets.cpp
)

add_executable(OpenGLProject ${SOURCES} ${IMGUI_SOURCES})

target_link_libraries(OpenGLProject
    PRIVATE
        OpenGL::GL
        glfw
        glew
)

add_custom_command(
  TARGET OpenGLProject
  POST_BUILD
  COMMAND OpenGLProject
)


add_custom_target(lint
    COMMAND clang-format --style=file src/*.cpp src/*.hh -i
 )
