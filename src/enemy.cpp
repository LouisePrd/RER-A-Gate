#include <iostream>
#include <algorithm>
#include <time.h>
#include "enemy.hpp"
#include "map.hpp"
#include "itdReader.hpp"

void Enemy::move(int sizex, int sizey, Map map, double elapsedTime)
{
    timeAccumulator += elapsedTime;
    const double movementInterval = 0.5;
    if (timeAccumulator < movementInterval)
        return;
    timeAccumulator = 0;

    int id = this->x + this->y * sizex;
    caseMap currentCase = map.listCases[id];
    int xCase = currentCase.x;
    int yCase = currentCase.y;

    std::vector<caseMap> adjacentCases;
    if (xCase > 0)
        adjacentCases.push_back(map.listCases[id - 1]);
    if (xCase < sizex - 1)
        adjacentCases.push_back(map.listCases[id + 1]);
    if (yCase > 0)
        adjacentCases.push_back(map.listCases[id - sizex]);
    if (yCase < sizey - 1)
        adjacentCases.push_back(map.listCases[id + sizex]);

    // chemin le plus court entre les cases adjacentes et la sortie
    caseMap nextCase = currentCase;
    for (caseMap adjacentCase : adjacentCases)
    {
        if (adjacentCase.type == typeCase::out)
        {
            nextCase = adjacentCase;
            break;
        }
    }
    
}
