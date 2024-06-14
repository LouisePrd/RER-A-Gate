#include "App.hpp"
#include "itdReader.hpp"
#include "map.hpp"
#include "enemy.hpp"
#include "node.hpp"

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

Map map;
int sizex = 0;
int sizey = 0;
float divCasesx = 0;
float divCasesy = 0;
Enemy enemyTest{0, 0, 10, 1, 1};


App::App() : _previousTime(0.0), _viewSize(2.0)
{
    mappingTexture();
}

void App::setup()
{
    // Initialisation de la fenêtre
    glViewport(0, 0, _width, _height);

    std::cout << "Launching map" << std::endl;
    img::Image baseMap{img::load(make_absolute_path("images/mapRGB.png", true), 3, true)};
    sizex = baseMap.width();
    sizey = baseMap.height();
    divCasesx = 1/(float)sizex;
    divCasesy = 1/(float)sizey;
    std::vector<std::vector<int>> nodes = checkMap();
    checkImage(baseMap);
    map = compareMapItd(getNodes(nodes), checkImage(baseMap));

    // Transformation des chemins en graphe
    WeightedGraph graph = build_from_adjacency_matrix(createGraph(map));
    dijkstra(graph, 0, 7);

    glClearColor(0.0f, 0.745f, 0.682f, 1.0f); // #00BEBF
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.EnableBlending(true);

}

void App::update()
{
    const double currentTime{glfwGetTime()};
    const double elapsedTime{currentTime - _previousTime};
    _previousTime = currentTime;

    static double startTime = -1.0;
    if (startTime < 0.0)
        startTime = currentTime;

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
    glRotatef(_angle, 0.0f, 0.0f, 1.0f);
    draw_quad_with_texture(_texture);
    glPopMatrix();

    TextRenderer.SetTextSize(SimpleText::FontSize::SIZE_64);
    TextRenderer.Label("RER A GATE", _width / 2, 60, SimpleText::CENTER);
    displayMap(map);
    displayEnemy(0, enemyTest);

    TextRenderer.Render();
}

void App::key_callback(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/)
{
}

void App::mouse_button_callback(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        int width, height;
        glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
        const float aspectRatio{width / (float)height};
        float posMapX = (xpos / width - 0.5f) * _viewSize * aspectRatio;
        float posMapY = (0.5f - ypos / height) * _viewSize;
        std::cout << "Position de la souris : (" << posMapX << ", " << posMapY << ")" << std::endl;
        if ((posMapX >= -0.5f && posMapX <= 0.5f) && (posMapY >= -0.5f && posMapY <= 0.5f))
        {
            std::cout << "Dans la map" << std::endl;
            // code pour ajouter une tour
        }
    }
}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/)
{
}

void App::cursor_position_callback(double /*xpos*/, double /*ypos*/)
{
}

void App::size_callback(GLFWwindow* window, int width, int height)
{
    _width = width;
    _height = height;

    int viewport_width {};
    int viewport_height {};

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
// pas implémentée : opti ?
void App::mappingTexture()
{
    img::Image grassTile1{img::load(make_absolute_path("images/grass-tiles/grass-tile-1.png", true), 3, true)};
    img::Image grassTile2{img::load(make_absolute_path("images/grass-tiles/grass-tile-2.png", true), 3, true)};
    img::Image grassTile3{img::load(make_absolute_path("images/grass-tiles/grass-tile-3.png", true), 3, true)};
    img::Image railsDroits1{img::load(make_absolute_path("images/rails-tiles/rails-droit-1.png", true), 3, true)};
    img::Image in{img::load(make_absolute_path("images/in-out/in.png", true), 3, true)};
    img::Image out{img::load(make_absolute_path("images/in-out/out.png", true), 3, true)};
    img::Image path{img::load(make_absolute_path("images/enemy.png", true), 3, true)};
    img::Image enemy{img::load(make_absolute_path("images/enemy.png", true), 3, true)};
    _texturesMap.push_back(loadTexture(grassTile1));
    _texturesMap.push_back(loadTexture(grassTile2));
    _texturesMap.push_back(loadTexture(grassTile3));
    _texturesMap.push_back(loadTexture(railsDroits1));
    _texturesMap.push_back(loadTexture(in));
    _texturesMap.push_back(loadTexture(out));
    _texturesMap.push_back(loadTexture(path));
    _texturesEnemy.push_back(loadTexture(enemy));

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
        default:
            break;
        }

        displayElement(textureId, -0.5f + (i % sizex) * 0.125f, -0.5f + (i / sizey) * 0.125f, -0.5f + (i % sizex + 1) * 0.125f, -0.5f + (i / sizey) * 0.125f, -0.5f + (i % sizex + 1) * 0.125f, -0.5f + (i / sizey + 1) * 0.125f, -0.5f + (i % sizex) * 0.125f, -0.5f + (i / sizey + 1) * 0.125f);
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
    glVertex2f(-0.5f + enemy.x * divCasesx, -0.5f + adjustedY * divCasesy);
    glTexCoord2d(1, 0);
    glVertex2f(-0.5f + (enemy.x + 1) * divCasesx, -0.5f + adjustedY * divCasesy);
    glTexCoord2d(1, 1);
    glVertex2f(-0.5f + (enemy.x + 1) * divCasesx, -0.5f + (adjustedY + 1) * divCasesy);
    glTexCoord2d(0, 1);
    glVertex2f(-0.5f + enemy.x * divCasesx, -0.5f + (adjustedY + 1) * divCasesy);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    //std::cout << "Position de l'ennemi : (" << enemy.x << ", " << enemy.y << ")" << std::endl;
}
