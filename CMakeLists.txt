cmake_minimum_required(VERSION 3.8)
project(HmahTetris C)

unset(CMAKE_CXX_COMPILER)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(TETRIS_TARGET "tetris")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(NDEBUG ON)

add_subdirectory(src)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})