#pragma once

#include "map.hpp"
#include "node.hpp"
#include "itdReader.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Enemy
{
    int health;
    float speed;
    int loot;
    double x, y;
    int type;
    int nodeStart;
    
    double timeAccumulator = 0.0;
    unsigned long currentIndex = 0;
    std::vector<int> path;
    bool newPath = true;
    bool isShot = false;
    double shotTime;
    
    double startX, startY;
    double interpolationTime = 0.0;
    double interpolationDuration = 0.5;

    void moveIntoGraph(WeightedGraph graph, int start, int end, Map map, double elapsedTime);
    void moveX(double &x, double xOut, double elapsedTime);
    void moveY(double &y, double yOut, double elapsedTime);
};
