#pragma once

#include <vector>

struct Tower
{
    std::string type;
    int power;
    int scope;
    int speed;
    int price;
};

// --- Méthode Chebyshev : portée en diagonale
int chebyshev(int x1, int y1, int x2, int y2);

