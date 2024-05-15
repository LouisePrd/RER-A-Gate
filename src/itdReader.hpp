#pragma once

#include <vector>

struct Node
{
    int id;
    int x;
    int y;
    std::vector<int> noeudsConnectes;
};

void checkMap();