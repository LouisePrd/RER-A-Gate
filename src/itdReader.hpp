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

struct Node
{
    int id;
    int x;
    int y;
    std::vector<int> noeudsConnectes;
};

struct typeByColor {
    typeCase type;
    std::array<float, 3> color;
};

std::vector<Node> getNodes(std::vector<std::vector<int>> nodes);
std::vector<std::vector<int>> checkMap();
void displayEnum(typeCase type);
std::array<typeByColor, 3> getItdAllTypes();