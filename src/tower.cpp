#include <glad/glad.h>
#include "glad/glad.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <algorithm>

#include "tower.hpp"
#include "map.hpp"
#include "enemy.hpp"
#include "App.hpp"

int chebyshev(int x1, int y1, int x2, int y2)
{
    return std::max(std::abs(y2 - y1), std::abs(x2 - x1));
}

bool Tower::testRange(float x1, float y1, float x2, float y2)
{
    return chebyshev(x1, y1, x2, y2) <= scope;
}

Map newTower(float posMapX, float posMapY, int selectedTowerType, Map map, float sizeDisplay, int &totalMoney, std::vector<Tower> &towers)
{
    float limit = sizeDisplay / 2.f;
    int gridX = static_cast<int>((posMapX + limit) * map.width);
    int gridY = static_cast<int>((posMapY + limit) * map.height);

    if (posMapX < -limit || posMapX > limit || posMapY < -limit || posMapY > limit)
    {
        return map;
    }

    int index = gridX + gridY * map.width;
    int clickedCase = map.listCases[index].type;

    if (clickedCase != typeCase::path && clickedCase != typeCase::in && clickedCase != typeCase::out && selectedTowerType != -1)
    {
        Tower selectedTower;
        switch (selectedTowerType)
        {
        // Les stats sont en WIP
        case 0:
            selectedTower = {"towerA", 5, 2, 10, map.listPrices[0]};
            break;
        case 1:
            selectedTower = {"towerB", 10, 3, 7, map.listPrices[1]};
            break;
        case 2:
            selectedTower = {"towerC", 20, 5, 4, map.listPrices[2]};
            break;
        default:
            return map;
        }

        if (totalMoney >= selectedTower.price)
        {
            totalMoney -= selectedTower.price;
            map.listCases[index].type = static_cast<typeCase>(selectedTowerType + typeCase::towerA);
            selectedTower.x = map.listCases[index].x;
            selectedTower.y = map.listCases[index].y;
            towers.push_back(selectedTower);
        }
        else
        {
            std::cout << "Pas assez de money" << std::endl;
        }
    }

    return map;
}

void App::shootEnemies(Tower &tower, Enemy &enemy, float deltaTime)
{
    tower.timeSinceLastShot += deltaTime;
    if (tower.testRange(tower.x, tower.y, enemy.x, enemy.y))
    {
        if (tower.timeSinceLastShot >= tower.pace)
        {
            if (enemy.health > 0)
            {
                enemy.health -= tower.power;
                bullet newBullet = {tower.x, tower.y, enemy.x, enemy.y, true};
                tower.bullets.push_back(newBullet);
                tower.timeSinceLastShot = 0;

                enemy.isShot = true;
                enemy.shotTime = glfwGetTime();

                if (enemy.health <= 0){
                    totalMoney += enemy.loot;
                    enemy.health = 0;
                }  
            }
        }
    }
}