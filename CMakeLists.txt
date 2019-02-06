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
add_subdirectory(external/glfw)

# assimp
option(DIRECTX OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(BUILD_SHARED_LIBS OFF)
add_subdirectory(external/assimp)

#Search source files
file(GLOB SOURCE
  "src/*.cpp"
  "src/*.vs"
  "src/*.fs"
  "src/*.h"
)
# add source files
add_executable(open-model-viewer
  ${SOURCE}
  src/glad.c
  src/tinyfiledialogs.c
)

# link libraries
target_link_libraries(open-model-viewer
  glfw
  assimp
)

# include headerfiles
include_directories(
  ${CMAKE_SOURCE_DIR}/includes
  external/glfw/include
  external/assimp/include
)

file(GLOB SHADERS
  "src/*.fs"
  "src/*.vs"
)

# copy the resource folder
file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# copied from: github.com/JoeyDeVries/LearnOpenGL
set(NAME "open-model-viewer")
foreach(SHADER ${SHADERS})
  if(WIN32)
    file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  elseif(UNIX AND NOT APPLE)
    file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/${CHAPTER})
  elseif(APPLE)
    get_filename_component(SHADERNAME ${SHADER} NAME)
    makeLink(${SHADER} ${CMAKE_CURRENT_BINARY_DIR}/bin/${CHAPTER}/${SHADERNAME} ${NAME})
  endif(WIN32)
endforeach(SHADER)