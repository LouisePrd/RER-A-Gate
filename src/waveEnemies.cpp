#include "waveEnemies.hpp"


Wave createWave(int xStart, int yStart, int nbEnemies) {
    Wave wave;
    wave.spawnInterval = 2;

    for (int i = 0; i < nbEnemies; i++) {
        Enemy enemy;
        enemy.health = 100;
        enemy.speed = rand() % 4 + 0.5;
        enemy.loot = rand() % 50 + 50;
        enemy.x = xStart;
        enemy.y = yStart;
        wave.enemies.push_back(enemy);
    }

    return wave;
}
