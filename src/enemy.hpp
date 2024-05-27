#pragma once

#include <vector>

struct Enemy
{
    int health;
    int speed;
    int loot;
};

void moveOneEnemy(Enemy &enemies, int x, int y);