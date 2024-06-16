#include <iostream>
#include <algorithm>
#include <time.h>
#include "enemy.hpp"
#include "map.hpp"
#include "itdReader.hpp"


void Enemy::moveIntoGraph(WeightedGraph graph, int start, int end, Map map, double elapsedTime)
{
    timeAccumulator += elapsedTime;
    const double movementInterval = speed;

    if (timeAccumulator < movementInterval)
        return;

    timeAccumulator = 0;

    if (newPathNeeded)
    {
        path.clear();
        auto result = dijkstra(graph, start, end);

        for (int at = end; at != -1; at = result[at].second)
        {
            path.push_back(at);
        }
        std::reverse(path.begin(), path.end());
        path.erase(path.begin());

        newPathNeeded = false;
        currentIndex = 0;
    }

    if (currentIndex >= path.size())
        return;

    int nextNode = path[currentIndex];
    for (int i = 0; i < map.listCases.size(); i++)
    {
        if (map.listCases[i].node.id == nextNode)
        {
            if (this->x != map.listCases[i].node.x)
            {
                moveX(this->x, map.listCases[i].node.x);
            }
            else if (this->y != map.listCases[i].node.y)
            {
                moveY(this->y, map.listCases[i].node.y);
            }

            if (this->x == map.listCases[i].node.x && this->y == map.listCases[i].node.y)
                currentIndex++;

            break;
        }
    }
}


void Enemy::moveX(int &x, int xOut)
{
    if (x < xOut)
        x++;
    else if (x > xOut)
        x--;
}

void Enemy::moveY(int &y, int yOut)
{
    if (y < yOut)
        y++;
    else if (y > yOut)
        y--;
}