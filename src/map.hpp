#pragma once

#include "img/img.hpp"
#include "itdReader.hpp"
#include <vector>
#include <array>

struct pixel {
    int x;
    int y;
    std::array<float, 3> color;
};

struct Map {
    int height;
    int width;
    std::vector<std::vector<pixel>> listPixels;
};

Map checkImage(img::Image &baseMap);