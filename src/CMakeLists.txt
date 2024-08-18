cmake_minimum_required(VERSION 3.8)

set(SOURCES
  # Entry
  src/main.c

  # Deps
  deps/src/gl3w.c

  # Core
  src/app.c
  src/context.c
  src/cursor.c
  src/curve.c
  src/drawable.c
  src/field.c
  src/figure.c
  src/input.c
  src/material.c
  src/node.c
  src/scene.c
  src/sprite.c
  src/texture.c
  src/timing.c
  src/transform.c

  # Shaders
  src/shaders/basic.c
  src/shaders/image.c

  # Scenes
  src/scenes/index.c
  src/scenes/intro.c
  src/scenes/menu.c
  src/scenes/tetris.c
)

add_executable(${TETRIS_TARGET} ${SOURCES})

target_include_directories(${TETRIS_TARGET} PRIVATE include)
target_include_directories(${TETRIS_TARGET} PRIVATE deps/include)

# Add CPM
include(cmake/CPM.cmake)

# Add SDL
CPMAddPackage(
  NAME SDL3
  GITHUB_REPOSITORY libsdl-org/SDL
  GIT_TAG 29f0fd33dc4683539c6c041baeb3bf14dd84a686
  OPTIONS
    "SDL_STATIC ON"
    "SDL_SHARED OFF"
)
message("SDL3_SOURCE_DIR = ${SDL3_SOURCE_DIR}")
target_link_libraries(${TETRIS_TARGET} PRIVATE SDL3::SDL3)
target_include_directories(${TETRIS_TARGET} PRIVATE ${SDL3_SOURCE_DIR}/include)

# Add CGLM
set(CGLM_SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/cglm-src)
CPMAddPackage(
  NAME cglm
  GITHUB_REPOSITORY recp/cglm
  VERSION 0.9.4
  OPTIONS "CGLM_STATIC ON"
)
target_link_libraries(${TETRIS_TARGET} PRIVATE cglm)
target_include_directories(${TETRIS_TARGET} PRIVATE ${CGLM_SOURCE_DIR}/include)
