#include <glad/glad.h>
#include "glad/glad.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <algorithm>

#include "tower.hpp"


int chebyshev(int x1, int y1, int x2, int y2) {
    return std::max(std::abs(y2 - y1), std::abs(x2 - x1));
}

// ---- Instances des tours
// Valeurs temporaires arbitraires en score sur 10
// ==== équilibrée
Tower typeA { "TypeA", 7, 7, 7, 10 };
// ==== + puissante - cadence
Tower typeB { "TypeB", 10, 7, 3, 8 };
// ==== - puissante + cadence
Tower typeC { "TypeC", 5, 5, 10, 8 };
