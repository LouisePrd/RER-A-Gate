#include <iostream>
#include <time.h>
#include "enemy.hpp"
#include "map.hpp"
#include "itdReader.hpp"


void Enemy::moveIntoGraph(WeightedGraph graph, int start, int end, Map map, double elapsedTime)
{
    timeAccumulator += elapsedTime;

    if (timeAccumulator < speed)
        return;

    timeAccumulator = 0;

    if (newPath)
    {
        path.clear();
        auto result = dijkstra(graph, start, end);

        for (int at = end; at != -1; at = result[at].second)
        {
            path.push_back(at);
        }
        std::reverse(path.begin(), path.end());
        path.erase(path.begin());

        newPath = false;
        currentIndex = 0;

        if (!path.empty())
        {
            startX = x;
            startY = y;
        }
    }

    if (currentIndex >= path.size())
        return;

    int nextNode = path[currentIndex];
    for (unsigned long i = 0; i < map.listCases.size(); i++)
    {
        if (map.listCases[i].node.id == nextNode && isShot == false)
        {
            if (this->x != map.listCases[i].node.x)
            {
                moveX(this->x, map.listCases[i].node.x, elapsedTime);
            }
            if (this->y != map.listCases[i].node.y)
            {
                moveY(this->y, map.listCases[i].node.y, elapsedTime);
            }

            if (this->x == map.listCases[i].node.x && this->y == map.listCases[i].node.y)
            {
                currentIndex++;
                interpolationTime = 0;
                startX = x;
                startY = y;
            }

            break;
        }
    }
}



void Enemy::moveX(double &x, double xOut, double elapsedTime)
{
    if (x == xOut) return;

    interpolationTime += elapsedTime;
    if (interpolationTime > interpolationDuration)
    {
        interpolationTime = interpolationDuration;
    }

    double t = interpolationTime / interpolationDuration;
    x = startX + t * (xOut - startX);

    if (interpolationTime >= interpolationDuration)
    {
        x = xOut;
        interpolationTime = 0;
        startX = xOut;
    }
}


void Enemy::moveY(double &y, double yOut, double elapsedTime)
{
    if (y == yOut) return;

    interpolationTime += elapsedTime;
    if (interpolationTime > interpolationDuration)
    {
        interpolationTime = interpolationDuration;
    }

    double t = interpolationTime / interpolationDuration;
    y = startY + t * (yOut - startY);

    if (interpolationTime >= interpolationDuration)
    {
        y = yOut;
        interpolationTime = 0;
        startY = yOut;
    }
}