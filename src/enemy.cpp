#include <iostream>
#include <algorithm>
#include <time.h>
#include "enemy.hpp"
#include "map.hpp"
#include "itdReader.hpp"

void Enemy::move(int sizex, int sizey, Map map)
{
    int id = this->x + this->y * sizex;
    caseMap currentCase = map.listCases[id];
    int xCase = currentCase.x;
    int yCase = currentCase.y;
    std::cout << "L'ennemi est en position (" << xCase << ", " << yCase << ")" << std::endl;

    std::vector<caseMap> adjacentCases;
    if (xCase > 0)
        adjacentCases.push_back(map.listCases[id - 1]);
    if (xCase < sizex - 1)
        adjacentCases.push_back(map.listCases[id + 1]);
    if (yCase > 0)
        adjacentCases.push_back(map.listCases[id - sizex]);
    if (yCase < sizey - 1)
        adjacentCases.push_back(map.listCases[id + sizex]);

    for (caseMap c : adjacentCases)
    {
        std::cout << "Case adjacente : ";
        displayEnum(c.type);
    }

    for (caseMap c : adjacentCases)
    {
        if (c.type == path && c.x != this->x - 1 && c.y != this->y - 1)
        {
            this->x = c.x;
            this->y = c.y;
            std::cout << "L'ennemi se déplace en position (" << c.x << ", " << c.y << ")" << std::endl;
            break;
        }
        else if (c.type == out)
        {
            std::cout << "L'ennemi est arrivé à destination" << std::endl;
            break;
        }
    }
}
