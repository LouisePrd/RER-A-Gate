#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <img/img.hpp>
#include <array>
#include <unordered_map>

enum typeCase {
    path,
    in,
    out,
    none,
    towerA,
    towerB,
    towerC
};

struct typeByColor {
    typeCase type;
    std::array<float, 3> color;
};

std::vector<std::vector<int>> checkMap();
std::string getNameMap();
std::string getNbNodes();
void displayEnum(typeCase type);
std::array<typeByColor, 3> getItdAllTypes();