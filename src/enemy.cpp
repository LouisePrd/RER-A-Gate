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

    std::vector<caseMap> possibleCases;
    for (caseMap c : adjacentCases)
    {
        if (c.type == typeCase::path)
            possibleCases.push_back(c);
    }

    std::cout << "Possible cases: " << possibleCases.size() << std::endl;

}

// on récupère le graphe
void Enemy::moveIntoGraph(WeightedGraph graph, int start, int end)
{
    std::unordered_map<int, std::pair<float, int>> distances = dijkstra(graph, start, end);
    std::vector<int> path;
    int current = end;
    while (current != start)
    {
        path.push_back(current);
        current = distances[current].second;
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    for (int i : path)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}