#pragma once
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "vtparse/vtparse.h"
#include "freetype/include/ft2build.h"
#include FT_FREETYPE_H

#include <iomanip>
#include <cmath>
#include <iostream>
#include <string.h>
#include <sstream>
#include <vector>
#include <deque>
#include <cstddef>

#define APPLICATION_NAME DockTerm

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

#define LOG(...) std::cout << __VA_ARGS__ << std::endl;   