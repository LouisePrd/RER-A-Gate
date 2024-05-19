#include "map.hpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

void drawMap(img::Image &baseMap)
{ 
    std::vector<glm::vec3> map;
    int size = baseMap.width() * baseMap.height();
    for (int i = 0; i < size; i++)
    {
        map.push_back(glm::vec3(i % baseMap.width(), i / baseMap.width(), 0));
    }
}