cmake_minimum_required(VERSION 3.1)

project(fighting_tutorial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

set (FIGHTING_SRCS
    src/AppEntry.cpp)

add_executable(fighting_tutorial ${FIGHTING_SRCS})

# glew
find_library(GL_LIB GL)
find_library(GLEW_LIB GLEW)
message(${GL_LIB})
message(${GLEW_LIB})

# SDL2
find_library(SDL_LIB SDL2 REQUIRED)
message(${SDL_LIB})
target_link_libraries(fighting_tutorial SDL2 GL GLEW)
