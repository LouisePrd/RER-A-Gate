#pragma once

#include "map.hpp"
#include "node.hpp"
#include "itdReader.hpp"

#include <glad/glad.h>
#include <vector>
#include "img/img.hpp"
#include <glm/glm.hpp>
#include <array>
#include <unordered_map>

struct Enemy
{
    int health;
    int speed;
    int loot;
    int x;
    int y;
    double timeAccumulator = 0.0;

    void moveIntoGraph(WeightedGraph graph, int start, int end, Map map, double elapsedTime);
    void moveX(int &x, int xOut);
    void moveY(int &y, int yOut);
};
