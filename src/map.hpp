#pragma once

#include "img/img.hpp"
#include "itdReader.hpp"
#include <vector>

struct Map {
    int height;
    int width;
    std::vector<Node> nodes;
    std::vector<std::vector<int>> map;
};

void drawMap(img::Image &baseMap);