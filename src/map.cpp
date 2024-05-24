#include "map.hpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "itdReader.hpp"

Map checkImage(img::Image &baseMap)
{
    std::vector<std::vector<pixel>> listPixels;
    int size = baseMap.width() * baseMap.height();
    for (int i = 0; i < size; i++)
    {
        int x = i % baseMap.width();
        int y = i / baseMap.width();
        glm::vec3 color;
        color.r = baseMap.data()[i * 3];
        color.g = baseMap.data()[i * 3 + 1];
        color.b = baseMap.data()[i * 3 + 2];
        std::cout << "Pixel (" << x << ", " << y << "): " << std::endl;
        pixel currentPixel {x, y, {color.r, color.g, color.b}};
        listPixels.push_back({currentPixel});
    }

    Map map {static_cast<int>(baseMap.height()), static_cast<int>(baseMap.width()), listPixels};

    return map;
}