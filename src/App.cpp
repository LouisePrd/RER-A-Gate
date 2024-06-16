#include "App.hpp"
#include "itdReader.hpp"
#include "map.hpp"
#include "enemy.hpp"
#include "node.hpp"
#include "userInterface.hpp"
#include "waveEnemies.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include <sstream>
#include <fstream>
#include <iostream>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"
#include "itdReader.hpp"
#include <stb_image/stb_image.h>
#include <map>

Enemy enemyTest{0, 0, 10, 1, 1, 0.0};

App::App() : _previousTime(0.0), _viewSize(2.0), map()
{
    mappingTexture();
}

void App::setup()
{
    // Initialisation de la fenêtre
    glViewport(0, 0, _width, _height);

    std::cout << "Launching map" << std::endl;
    img::Image baseMap{img::load(make_absolute_path("images/mapRGB-V2.png", true), 3, true)};
    sizex = baseMap.width();
    sizey = baseMap.height();
    divCasesx = sizeDisplay / (float)sizex;
    divCasesy = sizeDisplay / (float)sizey;

    std::vector<std::vector<int>> nodes = checkMap();
    map = compareMapItd(getNodes(nodes), checkImage(baseMap));
    map.graph = build_from_adjacency_matrix(createGraph(map));
    waveEnemies = createWave(1, 1, 5);

    glClearColor(0.0f, 0.745f, 0.682f, 1.0f); // #00BEBF
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
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
        for (int i = 0; i < waveEnemies.enemies.size(); i++)
        {
            waveEnemies.enemies[i].moveIntoGraph(map.graph, 0, 10, map, elapsedTime);
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

    TextRenderer.SetTextSize(SimpleText::FontSize::SIZE_128);
    TextRenderer.Label("RER A GATE", _width / 2, 220, SimpleText::CENTER);
    displayMap(map);

    if (started == true)
    {
        for (int i = 0; i < waveEnemies.enemies.size(); i++)
        {
            displayEnemy(0, waveEnemies.enemies[i]);
        }
    }
    displayTowerButtons();

    TextRenderer.Render();
}

void App::key_callback(int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        started = true;
    }
}

void App::mouse_button_callback(int button, int action, int /*mods*/) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        int width, height;
        glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
        const float aspectRatio{width / (float)height};
        float posMapX = (xpos / width - (sizeDisplay/2.f)) * _viewSize * aspectRatio;
        float posMapY = ((sizeDisplay/2.f) - ypos / height) * _viewSize;

        // ---- Si sur la map
        if ((posMapX >= -(sizeDisplay/2.f) && posMapX <= (sizeDisplay/2.f)) && (posMapY >= -(sizeDisplay/2.f) && posMapY <= (sizeDisplay/2.f))) {
            int clickedCase = map.listCases[(int)((posMapX + (sizeDisplay/2.f)) / divCasesx) + (int)((posMapY + (sizeDisplay/2.f)) / divCasesy) * sizex].type;

            if (clickedCase != typeCase::path && clickedCase != typeCase::in && clickedCase != typeCase::out && selectedTowerType != -1) {
                switch (selectedTowerType) {
                    case 0:
                        map.listCases[(int)((posMapX + (sizeDisplay/2.f)) / divCasesx) + (int)((posMapY + (sizeDisplay/2.f)) / divCasesy) * sizex].type = typeCase::towerA;
                        break;
                    case 1:
                        map.listCases[(int)((posMapX + (sizeDisplay/2.f)) / divCasesx) + (int)((posMapY + (sizeDisplay/2.f)) / divCasesy) * sizex].type = typeCase::towerB;
                        break;
                    case 2:
                        map.listCases[(int)((posMapX + (sizeDisplay/2.f)) / divCasesx) + (int)((posMapY + (sizeDisplay/2.f)) / divCasesy) * sizex].type = typeCase::towerC;
                        break;
                    default:
                        break;
                }
            }
        }
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

    // make sure the viewport matches the new window dimensions
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
        "images/in-out/out.png",
        "images/enemy.png",
        "images/tower-tiles/tower-A.png",
        "images/tower-tiles/tower-B.png",
        "images/tower-tiles/tower-C.png"
    };

    for (const std::string& path : texturePaths) {
        img::Image textureImage{img::load(make_absolute_path(path, true), 3, true)};
        _texturesMap.push_back(loadTexture(textureImage));
    }

    img::Image enemyImage{img::load(make_absolute_path("images/enemies/enemy1.png", true), 3, true)};
    _texturesEnemy.push_back(loadTexture(enemyImage));

    if (_texturesMap.size() == 0)
        std::cerr << "Error: no textures loaded" << std::endl;
    else
        std::cout << "Textures loaded (" << _texturesMap.size() << ")" << std::endl;
}

void App::displayMap(Map map)
{
    int textureId = 0;
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
            _texture = _texturesMap[7];
            textureId = 7;
            break;
        case typeCase::towerB:
            _texture = _texturesMap[8];
            textureId = 8;
            break;
        case typeCase::towerC:
            _texture = _texturesMap[9];
            textureId = 9;
            break;
        default:
            break;
        }

        displayElement(textureId, -(sizeDisplay/2.f) + (i % sizex) * divCasesx, -(sizeDisplay/2.f) + (i / sizey) * divCasesy, -(sizeDisplay/2.f) + (i % sizex + 1) * divCasesx, -(sizeDisplay/2.f) + (i / sizey) * divCasesy, -(sizeDisplay/2.f) + (i % sizex + 1) * divCasesx, -(sizeDisplay/2.f) + (i / sizey + 1) * divCasesy, -(sizeDisplay/2.f) + (i % sizex) * divCasesx, -(sizeDisplay/2.f) + (i / sizey + 1) * divCasesy);
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

void App::displayEnemy(int idTexture, Enemy enemy)
{
    float adjustedY = sizey - enemy.y - 1;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texturesEnemy[idTexture]);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(-(sizeDisplay/2.f) + enemy.x * divCasesx, -(sizeDisplay/2.f) + adjustedY * divCasesy);
    glTexCoord2d(1, 0);
    glVertex2f(-(sizeDisplay/2.f) + (enemy.x + 1) * divCasesx, -(sizeDisplay/2.f) + adjustedY * divCasesy);
    glTexCoord2d(1, 1);
    glVertex2f(-(sizeDisplay/2.f) + (enemy.x + 1) * divCasesx, -(sizeDisplay/2.f) + (adjustedY + 1) * divCasesy);
    glTexCoord2d(0, 1);
    glVertex2f(-(sizeDisplay/2.f) + enemy.x * divCasesx, -(sizeDisplay/2.f) + (adjustedY + 1) * divCasesy);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}