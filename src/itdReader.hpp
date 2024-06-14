#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <img/img.hpp>
#include <array>


enum typeCase {
    path,
    in,
    out,
    none
};

struct typeByColor {
    typeCase type;
    std::array<float, 3> color;
};

std::vector<std::vector<int>> checkMap();
void displayEnum(typeCase type);
std::array<typeByColor, 3> getItdAllTypes();