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
};

Map newTower(float x, float y, int selectedTower, Map map, float sizeDisplay, int &totalMoney);
// --- Méthode Chebyshev : distance en diagonale
int chebyshev(int x1, int y1, int x2, int y2);
