cmake_minimum_required(VERSION 3.14)

project(MemoriaIci VERSION 1.0)

# Especifica el estándar de C++ requerido
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "-static")


set(root ${CMAKE_SOURCE_DIR})
set(local_root ${CMAKE_CURRENT_SOURCE_DIR})
set(dependencies_directory ${root}/dependencies)
set(dependencies_dir ${root}/dependencies)
set(source_dir ${root}/src)

set(modules "") # Modules list


# -------------- Backend Import ----------------

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(src/)

list(APPEND modules ${SDL2_LIBRARIES})
list(APPEND modules ${GLEW_LIBRARIES})
list(APPEND modules ${OPENGL_LIBRARIES})

# ---------------- ImGui Import -------------------

set(IMGUI_PATH ${dependencies_dir}/imgui)

set(IMGUI_SOURCES
	${IMGUI_PATH}/imgui.cpp
	${IMGUI_PATH}/imgui_demo.cpp
	${IMGUI_PATH}/imgui_draw.cpp
	${IMGUI_PATH}/imgui_tables.cpp
	${IMGUI_PATH}/imgui_widgets.cpp
	# Backends
	${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
	${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
   )
   
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_PATH})
list(APPEND modules imgui)


# ---------------- imgui-filebrowser Import -------------------

set(IMFB_PATH ${dependencies_dir}/imgui-filebrowser)
include_directories(${IMFB_PATH})

# ----------------- ImPlot Import ---------------------

set(IMPLOT_PATH ${dependencies_dir}/implot)

set(IMPLOT_SOURCES
	${IMPLOT_PATH}/implot.cpp
	${IMPLOT_PATH}/implot_demo.cpp
	${IMPLOT_PATH}/implot_items.cpp
   )
add_library(implot STATIC ${IMPLOT_SOURCES})
target_include_directories(implot PUBLIC ${IMPLOT_PATH})
target_link_libraries(implot PUBLIC imgui)
list(APPEND modules implot)



# ----------------- Exec ----------------------
add_executable(${CMAKE_PROJECT_NAME} 
                            ${source_dir}/main.cpp
                            ${source_dir}/camera/Camera.cpp 
                            ${source_dir}/shader/Shader.cpp 
                            ${source_dir}/common.cpp 
                            ${source_dir}/mesh/mesh.cpp 
                            ${source_dir}/mesh/polyhedral.cpp
                            ${source_dir}/ui/ui.cpp
                            ${source_dir}/events/events.cpp
                            ${source_dir}/scene/scene.cpp
                            )

target_link_libraries(${CMAKE_PROJECT_NAME} ${modules})
