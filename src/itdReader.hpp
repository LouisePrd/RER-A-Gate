#pragma once

#include <vector>
#include <glm/glm.hpp>

struct Node
{
    int id;
    int x;
    int y;
    std::vector<int> noeudsConnectes;
};

std::vector<Node> getNodes(std::vector<std::vector<int>> nodes);
void checkMap();
std::vector<glm::vec3> loadMap(std::string path);
void transformMap(std::vector<glm::vec3> &map, std::vector<Node> nodes);