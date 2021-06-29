cmake_minimum_required(VERSION 3.1)

project(fighting_tutorial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

set (FIGHTING_SRCS
    src/AppEntry.cpp)

add_executable(fighting_tutorial ${FIGHTING_SRCS})

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/libs")

find_package(OpenGL)
message("OpenGL support libs: ${OPENGL_LIBRARIES}")

find_library(SDL2_LIB SDL2 REQUIRED)
find_library(GLEW_LIB glew32 GLEW REQUIRED)
target_link_libraries(fighting_tutorial ${SDL2_LIB} ${GLEW_LIB})

message(${SDL2_LIB})
message(${GLEW_LIB})

target_include_directories(fighting_tutorial PUBLIC libs/include)
target_link_libraries(fighting_tutorial ${OPENGL_LIBRARIES})

target_compile_definitions(fighting_tutorial PUBLIC
	"ASSETS_PATH=\"${CMAKE_SOURCE_DIR}/assets\"")
