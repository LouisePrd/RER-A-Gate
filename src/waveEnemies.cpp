#include "waveEnemies.hpp"


Wave createWave(int nbEnemies, int waveNumber, std::vector<std::pair<int, int>> startPositions, Map map){
    Wave wave;
    int nbSpawn = startPositions.size();
    std::cout << "Creating wave " << waveNumber << " with " << nbEnemies << " enemies" << std::endl;
    if (waveNumber > 5) {
        nbEnemies += rand() % 10; 
    }

    std::srand(static_cast<unsigned int>(std::time(0)));
    
    for (int i = 0; i < nbEnemies; i++) {
        Enemy enemy;
        if (waveNumber < 5) {
            enemy.health = 90;
            enemy.speed = 0.6 + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (2 - 0.6)));
            enemy.loot = rand() % 70;
            enemy.type = 0;
        } else {
            if (rand() % 2 == 0) {
                enemy.health = 90;
                enemy.speed = 0.6 + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (2 - 0.6)));
                enemy.loot = rand() % 40 + 20;
                enemy.type = 0;
            } else {
                enemy.health = 300;
                enemy.speed = 0.4 + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (1.5 - 0.4)));
                enemy.loot = rand() % 70 + 50;
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
