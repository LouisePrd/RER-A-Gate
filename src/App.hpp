#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <utility>
#include "utils.hpp"
#include "itdReader.hpp"
#include "map.hpp"
#include "enemy.hpp"
#include "node.hpp"
#include "waveEnemies.hpp"
#include "tower.hpp"

class App
{
public:
    App();
    
    // Main functions
    void setup();
    void update();
    void mappingTexture();

    // Display Map elements
    void displayBackGround();
    void displayMap(Map map);
    void displayElement(int idTexture, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
    void displayTower(int idTexture, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
    void displayEnemy(int idTexture, Enemy enemy);

    // Position functions
    std::pair<int, int> getEndPosition();
    std::vector<std::pair<int, int>> getStartPosition();

    // Game functions
    void shootEnemies(Tower &tower, Enemy &enemy, float deltaTime);
    void checkState();
    void resetGame();

    // GLFW callbacks binding
    void key_callback(int key, int scancode, int action, int mods);
    void mouse_button_callback(int button, int action, int mods);
    void scroll_callback(double xoffset, double yoffset);
    void cursor_position_callback(double xpos, double ypos);
    void size_callback(GLFWwindow* window, int width, int height);

    // User Interface
    bool isHovering(float x, float y, float width, float height);
    void displayButton(float buttonX, float buttonY, int idTexture);
    void displayTowerButtons();
    void displayMoney();
    void displayPrices();
    void displayScore();

    // Global variables
    int selectedTowerType = -1;
    int totalMoney = 1000;

private:
    void render();

    int _width{};
    int _height{};
    double _previousTime{};
    float _viewSize{};
    Map map{};

    // Add your variables here
    GLuint _texture{};
    GLuint backgroundTexture;

    // Map
    int sizex;
    int sizey;
    float divCasesx;
    float divCasesy;

    // Textures
    std::vector<GLuint> _texturesMap;
    std::vector<GLuint> _texturesEnemy;
    std::vector<GLuint> _texturesTower;

    // Enemies, Towers and Waves
    Wave waveEnemies;
    int indexWave = 0;
    std::vector<Tower> towersInMap;
    double waveDelay;

    // User Interface
    bool started = false;
    bool lost = false;
    float sizeDisplay = 1;

    SimpleText TextRenderer{};
};