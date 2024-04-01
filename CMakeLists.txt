cmake_minimum_required(VERSION 3.12)
project(Engine)

option(BUILD_DOC "Build documentation" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# Find OpenGL
find_package(OpenGL REQUIRED)

add_subdirectory(glew/build/cmake)
include_directories(glew/include)

add_subdirectory(glfw)
include_directories(glfw/include)

add_subdirectory(glm)
include_directories(glm)

file(GLOB SOURCES "src/*.cpp")
file(GLOB IMGUI_SOURCES "src/vendor/imgui/*.cpp")

file(GLOB HEADER_FILES "src/*.hh")
function(CHECK_PRAGMA_HEADER FILES)
  foreach(FILE ${${FILES}})
    file(STRINGS ${FILE} FILE_CONTENTS)
    list(GET FILE_CONTENTS 0 FIRST_LINE)
    if(NOT "${FIRST_LINE}" MATCHES "#pragma")
      message(FATAL_ERROR "${FILE} need a #pragma!! #################")
    endif()
  endforeach()
endfunction()
CHECK_PRAGMA_HEADER(HEADER_FILES)

find_package(Doxygen)
if (DOXYGEN_FOUND)

  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs_make/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  message("Doxygen build started")
  add_custom_target( doc_doxygen ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )

else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

add_executable(Engine ${SOURCES} ${IMGUI_SOURCES})

target_link_libraries(Engine
  PRIVATE
  OpenGL::GL
  glfw
  glew
  glm
)

add_custom_target(lint
  COMMAND clang-format --style=file src/*.cpp src/*.hh -i
)

add_custom_target(run
  COMMAND Engine
  DEPENDS Engine
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
