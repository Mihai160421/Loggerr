cmake_minimum_required(VERSION 3.10)
project(Loggerr VERSION 1.0)


message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
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
    ${CMAKE_SOURCE_DIR}/vendors/freetype/include
    ${CMAKE_SOURCE_DIR}/vendors/glew/include
)

add_library(imgui STATIC ${IMGUI_SOURCES})
add_library(vtparse STATIC ${VTPARSE_SOURCES})

# Setup GLFW
add_subdirectory(vendors/glfw)
include_directories(vendors/glfw/include)

# Setup FreeType
add_subdirectory(vendors/freetype)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(FT_WITH_HARFBUZZ OFF CACHE BOOL "" FORCE)
    set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
    set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
else()
endif()

#Create executable
add_executable(${PROJECT_NAME} ${SOURCE_FILE})

#Create PCH
target_precompile_headers(${PROJECT_NAME} PRIVATE  ${CMAKE_SOURCE_DIR}/inc/pch.h)

# Link GLFW OPENGL IMGUI VTPARSER
target_link_libraries(${PROJECT_NAME} PRIVATE glfw opengl32 imgui vtparse freetype)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/vendors/glew/glew32s.lib)

# Copy required assets
file(COPY ${CMAKE_SOURCE_DIR}/vendors/imgui/misc/fonts/ DESTINATION ${CMAKE_BINARY_DIR}/fonts)

