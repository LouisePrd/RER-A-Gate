#pragma once

#include "map.hpp"
#include "node.hpp"
#include "itdReader.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Enemy
{
    int health;
    int speed;
    int loot;
    int x;
    int y;
    double timeAccumulator = 0.0;
    int currentIndex = 0;
    std::vector<int> path;
    bool newPathNeeded = true;

    void moveIntoGraph(WeightedGraph graph, int start, int end, Map map, double elapsedTime);
    void moveX(int &x, int xOut);
    void moveY(int &y, int yOut);
};
