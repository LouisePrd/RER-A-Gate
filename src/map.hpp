#pragma once

#include "img/img.hpp"
#include "itdReader.hpp"
#include <vector>
#include <array>

enum typeCase {
    START,
    END,
    PATH,
};

struct caseMap {
    int x;
    int y;
    std::array<float, 3> color;
    Node node = {};
    typeCase type;
};


struct Map {
    int height;
    int width;
    std::vector<std::vector<caseMap>> listCases;
};

Map checkImage(img::Image &baseMap);
void compareMapItd(std::vector<Node> nodes, Map map);