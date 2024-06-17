#include "waveEnemies.hpp"


Wave createWave(int xStart, int yStart, int nbEnemies, int waveNumber) {
    Wave wave;

    for (int i = 0; i < nbEnemies; i++) {
        // enemy type 1
        if (waveNumber < 5) {
            Enemy enemy;
            enemy.health = 100;
            enemy.speed = rand() % 4 + 1;
            enemy.loot = rand() % 50 + 50;
            enemy.x = xStart;
            enemy.y = yStart;
            enemy.type = 0;
            wave.enemies.push_back(enemy);
            continue;
        } else {
            if (rand() % 2 == 0) {
                Enemy enemy;
                enemy.health = 100;
                enemy.speed = rand() % 4 + 1;
                enemy.loot = rand() % 40 + 40;
                enemy.x = xStart;
                enemy.y = yStart;
                enemy.type = 0;
                wave.enemies.push_back(enemy);
                continue;
            } else {
                Enemy enemy;
                enemy.health = 250;
                enemy.speed = rand() % 3 + 1;
                enemy.loot = rand() % 50 + 80;
                enemy.x = xStart;
                enemy.y = yStart;
                enemy.type = 1;
                wave.enemies.push_back(enemy);
                continue;
            }
            continue;
        }
    }

    return wave;
}
