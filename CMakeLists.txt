cmake_minimum_required(VERSION 3.10)
project(Loggerr VERSION 1.0)

file(GLOB SOURCE_FILE  ${CMAKE_SOURCE_DIR}/src/*.cpp)

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


# Set build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set optimization flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

# Optional: Enable warnings and C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic)


#Setup vtparse
set(VTPARSE_SOURCES
    vendors/vtparse/vtparse_table.c
    vendors/vtparse/vtparse.c
)

include_directories(
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/vendors/
    ${CMAKE_SOURCE_DIR}/vendors/imgui
    ${CMAKE_SOURCE_DIR}/vendors/imgui/backends
    ${CMAKE_SOURCE_DIR}/vendors/GLFW
    ${CMAKE_SOURCE_DIR}/vendors/vtparse
)


add_library(imgui STATIC ${IMGUI_SOURCES})
add_library(vtparse STATIC ${VTPARSE_SOURCES})

# Setup GLFW
add_subdirectory(vendors/glfw)
include_directories(vendors/glfw/include)

#Create executable
add_executable(${PROJECT_NAME} ${SOURCE_FILE})

#Create PCH
target_precompile_headers(${PROJECT_NAME} PRIVATE  ${CMAKE_SOURCE_DIR}/inc/pch.h)

# Link GLFW OPENGL IMGUI VTPARSER
target_link_libraries(${PROJECT_NAME} PRIVATE glfw opengl32 imgui vtparse)

# Copy required assets
file(COPY ${CMAKE_SOURCE_DIR}/vendors/imgui/misc/fonts/ DESTINATION ${CMAKE_BINARY_DIR}/fonts)

