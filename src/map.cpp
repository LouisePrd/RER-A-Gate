#include "map.hpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "itdReader.hpp"

void Map::setCaseType(int x, int y, typeCase type)
{
    listCases[y][x].type = type;
}

Map checkImage(img::Image &baseMap)
{
    std::vector<std::vector<caseMap>> listCases;
    int size = baseMap.width() * baseMap.height();
    for (int i = 0; i < size; i++)
    {
        int x = i % baseMap.width();
        int y = i / baseMap.width();
        glm::vec3 color;
        color.r = baseMap.data()[i * 3];
        color.g = baseMap.data()[i * 3 + 1];
        color.b = baseMap.data()[i * 3 + 2];
        caseMap currentcaseMap{x, y, {color.r, color.g, color.b}};
        listCases.push_back({currentcaseMap});
    }

    Map map{static_cast<int>(baseMap.height()), static_cast<int>(baseMap.width()), listCases};
    return map;
}

void compareMapItd(std::vector<Node> nodes, Map map)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        Node node = nodes[i]; // on récupère le noeud
        for (int j = 0; j < map.listCases.size(); j++)
        {
            for (int k = 0; k < map.listCases[j].size(); k++)
            {
                if (node.x == map.listCases[j][k].x && node.y == map.listCases[j][k].y)
                {
                    map.listCases[j][k].node = node;
                }
            }
        }
    }
}