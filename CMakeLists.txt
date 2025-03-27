cmake_minimum_required(VERSION 3.10)
project(Loggerr VERSION 1.0)

include_directories(
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/vendors/
    ${CMAKE_SOURCE_DIR}/vendors/imgui
    ${CMAKE_SOURCE_DIR}/vendors/imgui/backends
    ${CMAKE_SOURCE_DIR}/vendors/GLFW
)

file(GLOB SOURCE_FILE  ${CMAKE_SOURCE_DIR}/src/*.cpp)
message(STATUS "Source files: ${SOURCE_FILE}")
# Setup imgui
set(IMGUI_SOURCES
    vendors/imgui/imgui.cpp
    vendors/imgui/imgui_demo.cpp
    vendors/imgui/imgui_draw.cpp
    vendors/imgui/imgui_tables.cpp
    vendors/imgui/imgui_widgets.cpp
    vendors/imgui/backends/imgui_impl_glfw.cpp
    vendors/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})

# Setup GLFW
add_subdirectory(vendors/glfw)
include_directories(vendors/glfw/include)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})

# Link GLFW OPENGL and ImGUI 
target_link_libraries(${PROJECT_NAME} PRIVATE glfw opengl32 imgui)
