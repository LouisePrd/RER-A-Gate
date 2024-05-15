#pragma once

#include <vector>

struct Node
{
    int id;
    int x;
    int y;
    std::vector<int> noeudsConnectes;
};

std::vector<Node> getNodes(std::vector<std::vector<int>> nodes);
void checkMap();
