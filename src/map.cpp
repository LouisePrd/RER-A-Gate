#include "map.hpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "itdReader.hpp"

Map checkImage(img::Image &baseMap)
{
    std::vector<caseMap> listCases;
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
                break;
            }
            else
            {
                caseMap currentcaseMap{x, y, colorTab, typeCase::none};
                listCases.push_back({currentcaseMap});
                break;
            }
        }
    }

    // on affiche les cases
    /*for (unsigned long i = 0; i < listCases.size(); i++)
    {
        caseMap currentCase = listCases[i];
        std::cout << "Case " << i << " : " << currentCase.x << " " << currentCase.y << " ";
        std::cout << currentCase.color[0] << " " << currentCase.color[1] << " " << currentCase.color[2] << " ";
        displayEnum(currentCase.type);
    }*/

    Map map{static_cast<int>(baseMap.height()), static_cast<int>(baseMap.width()), listCases};
    return map;
}

Map compareMapItd(std::vector<Node> nodes, Map map)
{
    for (unsigned long i = 0; i < nodes.size(); i++)
    {
        Node node = nodes[i]; // on récupère le noeud
        for (unsigned long j = 0; j < map.listCases.size(); j++)
        {
            caseMap currentCase = map.listCases[j]; // on récupère la case
            if (node.x == currentCase.x && node.y == currentCase.y)
            {
                node.id = j;
            }
        }
    }
    return map;
}