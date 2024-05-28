#pragma once

#include <vector>
#include "map.hpp"
#include <glad/glad.h>

struct Enemy
{
    int health;
    int speed;
    int loot;
    int x;
    int y;

    void move(int sizex, int sizey, Map map);
};