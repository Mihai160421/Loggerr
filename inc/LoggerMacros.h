#pragma once
#include "Headers.h"

#define __PUSH_STYLE(...) ImGui::PushStyleVar(__VA_ARGS__)
#define __POP_STYLE(i)  { int it = i; while(it-- > 0) { ImGui::PopStyleVar(); } }