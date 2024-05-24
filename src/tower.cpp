#include <glad/glad.h>
#include "glad/glad.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <algorithm>

#include "tower.hpp"

unsigned chebyshev(int x1, int y1, int x2, int y2) {
    std::max(std::abs(y2 - y1), std::abs(x2 - x1));
}



