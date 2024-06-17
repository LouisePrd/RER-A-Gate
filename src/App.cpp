#include "App.hpp"
#include "itdReader.hpp"
#include "map.hpp"
#include "enemy.hpp"
#include "node.hpp"
#include "userInterface.hpp"
#include "waveEnemies.hpp"
#include "tower.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include <sstream>
#include <iostream>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"
#include <stb_image/stb_image.h>
#include <map>
#include <utility>

App::App() : _previousTime(0.0), _viewSize(2.0), map()
{
    mappingTexture();
}

void App::setup()
{
    glViewport(0, 0, _width, _height);

    std::cout << "Launching map" << std::endl;
    img::Image baseMap{img::load(make_absolute_path("images/" + getNameMap(), true), 3, true)};
    sizex = baseMap.width();
    sizey = baseMap.height();
    divCasesx = sizeDisplay / (float)sizex;
    divCasesy = sizeDisplay / (float)sizey;

    std::vector<std::vector<int>> nodes = checkMap();
    map = compareMapItd(getNodes(nodes), checkImage(baseMap));
    map.graph = build_from_adjacency_matrix(createGraph(map));
    getEndPosition();

    glClearColor(0.0f, 0.745f, 0.682f, 1.0f); // #00BEBF
    TextRenderer.ResetFont();
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.EnableBlending(true);
}

void App::update()
{
    const double currentTime{glfwGetTime()};
    const double elapsedTime{currentTime - _previousTime};
    _previousTime = currentTime;
    static double startTime = -1.0;

    if (started == true)
    {
        for (unsigned long i = 0; i < waveEnemies.enemies.size(); i++)
        {
            if (waveEnemies.enemies[i].isShot)
            {
                if (currentTime - waveEnemies.enemies[i].shotTime >= 1.0)
                {
                    waveEnemies.enemies[i].isShot = false;
                }
            }
            else
            {
                waveEnemies.enemies[i].moveIntoGraph(map.graph, 0, 10, map, elapsedTime * 1.5);
            }
        }
    }

    for (unsigned long i = 0; i < towersInMap.size(); i++)
    {
        for (unsigned long j = 0; j < waveEnemies.enemies.size(); j++)
        {
            shootEnemies(towersInMap[i], waveEnemies.enemies[j], elapsedTime);
        }
    }

    render();
}

void App::render()
{
    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glScalef(0.8f, 0.8f, 0.8f);
    glPopMatrix();

    // displayBackGround();
    TextRenderer.SetTextSize(SimpleText::FontSize::SIZE_128);
    TextRenderer.Label("RER A GATE", _width / 2, _height / 8, SimpleText::CENTER);
    displayMap(map);
    displayTowerButtons();
    displayMoney();
    displayPrices();
    checkState();

    TextRenderer.Render();
}

void App::key_callback(int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !started && !lost && !won)
    {
        waveEnemies = createWave(1, 1, 5);
        started = true;
        lost = false;
        won = false;
    }
}

void App::mouse_button_callback(int button, int action, int /*mods*/)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        int width, height;
        glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
        const float aspectRatio = width / static_cast<float>(height);
        float posMapX = (xpos / width - (sizeDisplay / 2.f)) * _viewSize * aspectRatio;
        float posMapY = ((sizeDisplay / 2.f) - ypos / height) * _viewSize;

        if (started == true)
            map = newTower(posMapX, posMapY, selectedTowerType, map, sizeDisplay, totalMoney, towersInMap);
    }
}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/)
{
}

void App::cursor_position_callback(double /*xpos*/, double /*ypos*/)
{
}

void App::size_callback(GLFWwindow *window, int width, int height)
{
    _width = width;
    _height = height;

    int viewport_width{};
    int viewport_height{};

    glfwGetFramebufferSize(window, &_width, &_height);
    glViewport(0, 0, viewport_width, viewport_height);

    const float aspectRatio{_width / (float)_height};

    // Change the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (aspectRatio > 1.0f)
    {
        glOrtho(-_viewSize / 2.0f * aspectRatio, _viewSize / 2.0f * aspectRatio, -_viewSize / 2.0f, _viewSize / 2.0f, -1.0f, 1.0f);
    }
    else
    {
        glOrtho(-_viewSize / 2.0f, _viewSize / 2.0f, -_viewSize / 2.0f / aspectRatio, _viewSize / 2.0f / aspectRatio, -1.0f, 1.0f);
    }
}

// fonction pour mapper texture et type de case
// nouvelle opti
void App::mappingTexture()
{
    std::vector<std::string> texturePaths = {
        "images/grass-tiles/grass-tile-1.png",
        "images/grass-tiles/grass-tile-2.png",
        "images/grass-tiles/grass-tile-3.png",
        "images/rails-tiles/rails-droit-1.png",
        "images/in-out/spawn.png",
        "images/in-out/end.png",
        "images/money.png"};

    std::vector<std::string> textureTowers = {
        "images/tower-tiles/tower-A.png",
        "images/tower-tiles/tower-B.png",
        "images/tower-tiles/tower-C.png"};

    for (const std::string &path : texturePaths)
    {
        img::Image textureImage{img::load(make_absolute_path(path, true), 3, true)};
        _texturesMap.push_back(loadTexture(textureImage));
    }

    for (const std::string &tower : textureTowers)
    {
        img::Image textureTowers{img::load(make_absolute_path(tower, true), 3, true)};
        _texturesTower.push_back(loadTexture(textureTowers));
    }

    img::Image enemyImage{img::load(make_absolute_path("images/enemies/enemy1.png", true), 3, true)};
    _texturesEnemy.push_back(loadTexture(enemyImage));

    backgroundTexture = loadTexture(img::Image{img::load(make_absolute_path("images/RER-A.png", true), 3, true)});

    if (_texturesMap.size() == 0)
        std::cerr << "Error: no textures loaded" << std::endl;
    else
        std::cout << "Textures loaded (" << _texturesMap.size() << ")" << std::endl;
}

void App::displayMap(Map map)
{
    int textureId = 0;
    int textureTowerId = 0;
    for (int i = 0; i < sizex * sizey; i++)
    {
        glEnable(GL_TEXTURE_2D);
        switch (map.listCases[i].type)
        {
        case typeCase::none:
            textureId = i % 3;
            _texture = _texturesMap[textureId];
            break;
        case typeCase::path:
            _texture = _texturesMap[3];
            textureId = 3;
            break;
        case typeCase::in:
            _texture = _texturesMap[4];
            textureId = 4;
            break;
        case typeCase::out:
            _texture = _texturesMap[5];
            textureId = 5;
            break;
        case typeCase::towerA:
            _texture = _texturesTower[0];
            textureTowerId = 0;
            break;
        case typeCase::towerB:
            _texture = _texturesTower[1];
            textureTowerId = 1;
            break;
        case typeCase::towerC:
            _texture = _texturesTower[2];
            textureTowerId = 2;
            break;
        default:
            break;
        }

        // si tower
        if (map.listCases[i].type == typeCase::towerA || map.listCases[i].type == typeCase::towerB || map.listCases[i].type == typeCase::towerC)
            displayTower(textureTowerId, -(sizeDisplay / 2.f) + (i % sizex) * divCasesx, -(sizeDisplay / 2.f) + (i / sizey) * divCasesy, -(sizeDisplay / 2.f) + (i % sizex + 1) * divCasesx, -(sizeDisplay / 2.f) + (i / sizey) * divCasesy, -(sizeDisplay / 2.f) + (i % sizex + 1) * divCasesx, -(sizeDisplay / 2.f) + (i / sizey + 1) * divCasesy, -(sizeDisplay / 2.f) + (i % sizex) * divCasesx, -(sizeDisplay / 2.f) + (i / sizey + 1) * divCasesy);
        else
            displayElement(textureId, -(sizeDisplay / 2.f) + (i % sizex) * divCasesx, -(sizeDisplay / 2.f) + (i / sizey) * divCasesy, -(sizeDisplay / 2.f) + (i % sizex + 1) * divCasesx, -(sizeDisplay / 2.f) + (i / sizey) * divCasesy, -(sizeDisplay / 2.f) + (i % sizex + 1) * divCasesx, -(sizeDisplay / 2.f) + (i / sizey + 1) * divCasesy, -(sizeDisplay / 2.f) + (i % sizex) * divCasesx, -(sizeDisplay / 2.f) + (i / sizey + 1) * divCasesy);
    }
}

void App::displayElement(int idTexture, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texturesMap[idTexture]);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(x1, y1);
    glTexCoord2d(1, 0);
    glVertex2f(x2, y2);
    glTexCoord2d(1, 1);
    glVertex2f(x3, y3);
    glTexCoord2d(0, 1);
    glVertex2f(x4, y4);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void App::displayTower(int idTexture, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texturesTower[idTexture]);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(x1, y1);
    glTexCoord2d(1, 0);
    glVertex2f(x2, y2);
    glTexCoord2d(1, 1);
    glVertex2f(x3, y3);
    glTexCoord2d(0, 1);
    glVertex2f(x4, y4);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void App::displayEnemy(int idTexture, Enemy enemy)
{
    if (enemy.isShot)
        glColor3ub(255, 0, 0);
    else
        glColor3ub(255, 255, 255);
    float adjustedY = sizey - enemy.y - 1;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texturesEnemy[idTexture]);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(-(sizeDisplay / 2.f) + enemy.x * divCasesx, -(sizeDisplay / 2.f) + adjustedY * divCasesy);
    glTexCoord2d(1, 0);
    glVertex2f(-(sizeDisplay / 2.f) + (enemy.x + 1) * divCasesx, -(sizeDisplay / 2.f) + adjustedY * divCasesy);
    glTexCoord2d(1, 1);
    glVertex2f(-(sizeDisplay / 2.f) + (enemy.x + 1) * divCasesx, -(sizeDisplay / 2.f) + (adjustedY + 1) * divCasesy);
    glTexCoord2d(0, 1);
    glVertex2f(-(sizeDisplay / 2.f) + enemy.x * divCasesx, -(sizeDisplay / 2.f) + (adjustedY + 1) * divCasesy);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

std::pair<int, int> App::getEndPosition()
{
    for (unsigned long i = 0; i < map.listCases.size(); i++)
    {
        if (map.listCases[i].type == typeCase::out)
        {
            return std::make_pair(map.listCases[i].x, map.listCases[i].y);
        }
    }
    return std::make_pair(-1, -1);
}

void App::displayBackGround()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(-1, -1);
    glTexCoord2d(1, 0);
    glVertex2f(1, -1);
    glTexCoord2d(1, 1);
    glVertex2f(1, 1);
    glTexCoord2d(0, 1);
    glVertex2f(-1, 1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void App::checkState()
{
    if (started == true)
    {
        for (unsigned long i = 0; i < waveEnemies.enemies.size(); i++)
        {
            displayEnemy(0, waveEnemies.enemies[i]);
            std::pair<int, int> endPosition = getEndPosition();
            if (waveEnemies.enemies[i].x == endPosition.first && waveEnemies.enemies[i].y == endPosition.second)
            {
                lost = true;
                started = false;
            }
            if (waveEnemies.enemies[i].health <= 0)
            {
                waveEnemies.enemies.erase(waveEnemies.enemies.begin() + i);
            }
        }
    }
    else
    {
        TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
        TextRenderer.SetTextSize(SimpleText::FontSize::SIZE_32);
        TextRenderer.Label("Press SPACE to start", _width / 2, _height / 7, SimpleText::CENTER);
        if (lost)
        {
            TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::RED);
            TextRenderer.SetTextSize(SimpleText::FontSize::SIZE_128);
            TextRenderer.Label("Game Over", _width / 2, _height / 2, SimpleText::CENTER);
        }
    }

    if (waveEnemies.enemies.size() == 0 && started == true)
    {
        won = true;
        started = false;
    }

    if (won)
    {
        TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::GREEN);
        TextRenderer.SetTextSize(SimpleText::FontSize::SIZE_128);
        TextRenderer.Label("You won", _width / 2, _height / 2, SimpleText::CENTER);
    }
}