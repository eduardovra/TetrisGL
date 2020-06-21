cmake_minimum_required(VERSION 2.8)
project(learnOpenGL)

#SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")

# Build GLAD
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PRIVATE external/glad/include)

# Locate glfw3
find_package(glfw3)
# Build CGLM
add_subdirectory(external/cglm)
# Build Assimp
#add_subdirectory(external/assimp)
# Im GUI
#add_subdirectory(external/cimgui)

set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_C_COMPILER "/usr/bin/clang")

#set(cimgui external/cimgui)
#set(imgui_impl ${cimgui}/imgui/examples)
#file(GLOB
#	cimgui_files
#	"${cimgui}/*.cpp" 
#	"${cimgui}/imgui/*.cpp"
#	"${imgui_impl}/imgui_impl_glfw.cpp"
#	"${imgui_impl}/imgui_impl_opengl3.cpp")

#add_library(cimgui ${cimgui_files})
#target_include_directories(cimgui PRIVATE ${cimgui} ${cimgui}/imgui ${imgui_impl} external/glad/include)
#target_compile_definitions(cimgui PRIVATE
#	IMGUI_IMPL_API=extern\ \"C\"
#	IMGUI_IMPL_OPENGL_LOADER_GLAD)

add_executable(tetris tetris/main.c tetris/mesh.h tetris/model.h tetris/shader.h tetris/camera.h)
target_include_directories(tetris PRIVATE external/glad/include external/stb)
target_link_libraries(tetris glfw GL X11 pthread Xrandr Xi m c glad ${CMAKE_DL_LIBS})
#target_compile_definitions(tetris PRIVATE
#	IMGUI_IMPL_API=\ )
