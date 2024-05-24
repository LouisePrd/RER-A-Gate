#pragma once

#include "img/img.hpp"
#include "itdReader.hpp"
#include <vector>
#include <array>

struct caseMap {
    int x;
    int y;
    std::array<float, 3> color;
    typeCase type;
    Node node = {};
};

struct Map {
    int height;
    int width;
    std::vector<caseMap> listCases;
};

Map checkImage(img::Image &baseMap);
void compareMapItd(std::vector<Node> nodes, Map map);