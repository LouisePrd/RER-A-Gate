#include "waveEnemies.hpp"


Wave createWave(int nbEnemies, int waveNumber, std::vector<std::pair<int, int>> startPositions, Map map){
    Wave wave;
    int nbSpawn = startPositions.size();
    std::cout << "Creating wave " << waveNumber << " with " << nbEnemies << " enemies" << std::endl;
    if (waveNumber > 5) {
        nbEnemies += rand() % 10; 
    }
    
    for (int i = 0; i < nbEnemies; i++) {
        Enemy enemy;
        if (waveNumber < 5) {
            enemy.health = 100;
            enemy.speed = rand() % 4 + 1;
            enemy.loot = rand() % 50;
            enemy.type = 0;
        } else {
            if (rand() % 2 == 0) {
                enemy.health = 100;
                enemy.speed = rand() % 4 + 1;
                enemy.loot = rand() % 30 + 20;
                enemy.type = 0;
            } else {
                enemy.health = 300;
                enemy.speed = rand() % 3 + 1;
                enemy.loot = rand() % 50 + 50;
                enemy.type = 1;
            }
        }

        int randSpawn = rand() % nbSpawn;
        std::pair<int, int> startPos = startPositions[randSpawn];
        enemy.x = startPos.first;
        enemy.y = startPos.second;
        for (int i = 0; i < map.listCases.size(); i++) {
            if (map.listCases[i].node.x == startPos.first && map.listCases[i].node.y == startPos.second) {
                enemy.nodeStart = map.listCases[i].node.id;
                break;
            }
        }

        wave.enemies.push_back(enemy);
    }

    return wave;
}
