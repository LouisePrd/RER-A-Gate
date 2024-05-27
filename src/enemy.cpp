#include <iostream>
#include <algorithm>

#include "enemy.hpp"

// ---- Instance des ennemis
// Valeurs temporaires arbitraires en score sur 10
Enemy enemy {7, 7, 3};

void moveOneEnemy(Enemy &enemies, int x, int y)
{
    std::cout << "L'ennemi est en position (" << x << ", " << y << ")" << std::endl;
    
}
