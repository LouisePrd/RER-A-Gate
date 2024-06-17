#pragma once

#include "enemy.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Wave {
    std::vector<Enemy> enemies;
    int spawnInterval;
};

Wave createWave(int nbEnemies, int waveNumber, std::vector<std::pair<int, int>> startPositions, Map map);
