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
        // on vérifie si la couleur correspond à un type de case
        std::array<float, 3> colorTab = {color.r, color.g, color.b};
        for (int j = 0; j < 3; j++)
        {
            if (colorTab == getItdAllTypes()[j].color && getItdAllTypes()[j].type != typeCase::none)
            {
                caseMap currentcaseMap{x, y, colorTab, getItdAllTypes()[j].type};
                listCases.push_back({currentcaseMap});
            } else {
                caseMap currentcaseMap{x, y, colorTab, typeCase::none};
                listCases.push_back({currentcaseMap});
            }
        }
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

    // on affiche les cases
    for (int i = 0; i < map.listCases.size(); i++)
    {
        for (int j = 0; j < map.listCases[i].size(); j++)
        {
            std::cout << "Case " << map.listCases[i][j].x << " " << map.listCases[i][j].y << " type ";
            displayEnum(map.listCases[i][j].type);
            
        }
    }
}