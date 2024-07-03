cmake_minimum_required(VERSION 3.14)

# Establece el nombre del proyecto
project(MemoriaIci VERSION 1.0)

# Especifica el estándar de C++ requerido
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(local_root ${CMAKE_CURRENT_SOURCE_DIR})


find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(src/)

set(IMGUI_PATH  "deps/imgui_and_implot")
file(GLOB IMGUI_SOURCES 
    ${IMGUI_PATH}/*.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
) 
add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})


add_executable(MemoriaIci 
                            src/main.cpp 
                            src/camera/Camera.cpp 
                            src/shader/Shader.cpp 
                            src/common.cpp 
                            src/models/models.cpp 
                            src/models/polyhedral.cpp 
                            src/mesh/mesh.cpp
                            src/mesh/octree.cpp
                            src/ui/ui.cpp
                            src/events/events.cpp
                            src/scene/scene.cpp
                            )

target_link_libraries(MemoriaIci     ImGui ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} )

