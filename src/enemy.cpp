#include <iostream>
#include <algorithm>
#include <time.h>
#include "enemy.hpp"
#include "map.hpp"
#include "itdReader.hpp"

// ---- Instance des ennemis
// Valeurs temporaires arbitraires en score sur 10

void Enemy::move(int sizex, int sizey, Map map)
{
    std::cout << "L'ennemi est en position (" << x << ", " << y << ")" << std::endl;
    int id = this->x + this->y * sizex;
    caseMap currentCase = map.listCases[id];
    int xCase = currentCase.x;
    int yCase = currentCase.y;
    std::cout << "L'ennemi est en position (" << xCase << ", " << yCase << ")" << std::endl;
    displayEnum(currentCase.type);

    // On regarde les cases adjacentes
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

}
