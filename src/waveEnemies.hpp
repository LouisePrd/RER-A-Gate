#pragma once

#include "enemy.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Wave {
    std::vector<Enemy> enemies;
    int spawnInterval;
};

Wave createWave(int xStart, int yStart, int nbEnemies);
