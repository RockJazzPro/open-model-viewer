cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 2.8)

project(open-model-viewer)

find_package(OpenGL REQUIRED)

# Error handling
if(P_BINARY_DIR STREQUAL P_SOURCE_DIR)
  message (FATAL_ERROR "Please select another Build Directory!")
endif()
if(P_SOURCE_DIR MATCHES " ")
  message("Your Source Directory contains white spaces. This can cause compiler errors.")
endif()

# disable documentation, tests and examples of glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# compile glfw
add_subdirectory(external/glfw-3.2.1)

# add source files
add_executable(open-model-viewer
  src/main.cpp
  src/glad.c
)

# link libraries
target_link_libraries(open-model-viewer
  glfw
)

# include headerfiles
include_directories(
  ${CMAKE_SOURCE_DIR}/includes
  external/glfw-3.2.1/include
)
