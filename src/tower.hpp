#pragma once

#include <vector>
#include <string>
#include "map.hpp"
#include "itdReader.hpp"

struct Tower
{
    std::string type;
    int power;
    int scope;
    int speed;
    int price;
    float x, y;

    bool testRange(float x1, float y1, float x2, float y2);
};

Map newTower(float x, float y, int selectedTower, Map map, float sizeDisplay, int &totalMoney, std::vector<Tower> &towers);
int chebyshev(int x1, int y1, int x2, int y2);
