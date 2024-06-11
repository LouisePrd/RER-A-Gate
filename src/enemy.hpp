#pragma once

#include <vector>
#include "map.hpp"
#include <glad/glad.h>

struct Enemy
{
    int health;
    int speed;
    int loot;
    int x;
    int y;
    double timeAccumulator = 0.0;

    void move(int sizex, int sizey, Map map, double elapsedTime);
    void moveIntoGraph(WeightedGraph graph, int start, int end);
};