cmake_minimum_required(VERSION 3.10)

# Establece el nombre del proyecto
project(MemoriaIci VERSION 1.0)

# Especifica el estándar de C++ requerido
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(TuProyecto src/main.cpp)
target_link_libraries(TuProyecto ${SDL2_LIBRARIES})

