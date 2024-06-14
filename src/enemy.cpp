#include <iostream>
#include <algorithm>
#include <time.h>
#include "enemy.hpp"
#include "map.hpp"
#include "itdReader.hpp"


void Enemy::moveIntoGraph(WeightedGraph graph, int start, int end, Map map, double elapsedTime)
{
    static int currentIndex = 0;
    timeAccumulator += elapsedTime;
    const double movementInterval = 0.5;
    if (timeAccumulator < movementInterval)
        return;
    timeAccumulator = 0;

    std::vector<int> chemin;
    for (auto const &[key, val] : dijkstra(graph, start, end))
    {
        chemin.push_back(key);
    }

    std::reverse(chemin.begin(), chemin.end());
    chemin.push_back(end);

    if (currentIndex >= chemin.size())
        return;

    int nextNode = chemin[currentIndex];
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