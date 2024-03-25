cmake_minimum_required(VERSION 3.10)

# Establece el nombre del proyecto
project(MemoriaIci VERSION 1.0)

# Especifica el estándar de C++ requerido
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(local_root ${CMAKE_CURRENT_SOURCE_DIR})


find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(src/)



add_executable(MemoriaIci src/main.cpp src/Camera.cpp src/common.cpp)
target_link_libraries(MemoriaIci ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

