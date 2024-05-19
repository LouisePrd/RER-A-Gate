#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <img/img.hpp>

struct Node
{
    int id;
    int x;
    int y;
    std::vector<int> noeudsConnectes;
};

std::vector<Node> getNodes(std::vector<std::vector<int>> nodes);
void checkMap();