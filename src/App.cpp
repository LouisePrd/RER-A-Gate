#include "App.hpp"
#include "itdReader.hpp"
#include "map.hpp"
#include "enemy.hpp"

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

Map map;
int sizex = 8;
int sizey = 8;

App::App() : _previousTime(0.0), _viewSize(2.0)
{
    img::Image grassTile1{img::load(make_absolute_path("images/grass-tiles/grass-tile-1.png", true), 3, true)};
    img::Image grassTile2{img::load(make_absolute_path("images/grass-tiles/grass-tile-2.png", true), 3, true)};
    img::Image grassTile3{img::load(make_absolute_path("images/grass-tiles/grass-tile-3.png", true), 3, true)};
    img::Image railsDroits1{img::load(make_absolute_path("images/rails-tiles/rails-droit-1.png", true), 3, true)};
    img::Image in{img::load(make_absolute_path("images/in-out/in.png", true), 3, true)};
    img::Image out{img::load(make_absolute_path("images/in-out/out.png", true), 3, true)};
    _textures.push_back(loadTexture(grassTile1));
    _textures.push_back(loadTexture(grassTile2));
    _textures.push_back(loadTexture(grassTile3));
    _textures.push_back(loadTexture(railsDroits1));
    _textures.push_back(loadTexture(in));
    _textures.push_back(loadTexture(out));
}

void App::setup()
{
    std::cout << "Launching map" << std::endl;
    img::Image baseMap{img::load(make_absolute_path("images/mapRGB.png", true), 3, true)};
    std::vector<std::vector<int>> nodes = checkMap();
    checkImage(baseMap);
    getNodes(nodes);
    map = compareMapItd(getNodes(nodes), checkImage(baseMap));

    // Set the clear color to a nice blue
    glClearColor(0.0f, 0.745f, 0.682f, 1.0f);
    // Setup the text renderer with blending enabled and white text color
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
    //_angle += 10.0f * elapsedTime;
    // _angle = std::fmod(_angle, 360.0f);

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

    int sizeTotal = sizex * sizey;
    // on met la texture sur chaque case
    for (int i = 0; i < sizeTotal; i++)
    {
        glEnable(GL_TEXTURE_2D);
        switch (map.listCases[i].type)
        {
        case typeCase::none:
            _texture = _textures[i % 3];
            break;
        case typeCase::path:
            _texture = _textures[3];
            break;
        case typeCase::in:
            _texture = _textures[4];
            break;
        case typeCase::out:
            _texture = _textures[5];
            break;
        default:
            break;
        }
        glBindTexture(GL_TEXTURE_2D, _texture);

        glColor3ub(255, 255, 255);
        glBegin(GL_QUADS);
        glTexCoord2d(0, 0);
        glVertex2f(-0.5f + (i % sizex) * 0.125f, -0.5f + (i / sizey) * 0.125f);

        glTexCoord2d(1, 0);
        glVertex2f(-0.5f + (i % sizex + 1) * 0.125f, -0.5f + (i / sizey) * 0.125f);

        glTexCoord2d(1, 1);
        glVertex2f(-0.5f + (i % sizex + 1) * 0.125f, -0.5f + (i / sizey + 1) * 0.125f);

        glTexCoord2d(0, 1);
        glVertex2f(-0.5f + (i % sizex) * 0.125f, -0.5f + (i / sizey + 1) * 0.125f);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }

    TextRenderer.SetTextSize(SimpleText::FontSize::SIZE_64);
    TextRenderer.Label("RER A GATE", _width / 2, 60, SimpleText::CENTER);

    // Without set precision
    // const std::string angle_label_text { "Angle: " + std::to_string(_angle) };
    // With c++20 you can use std::format
    // const std::string angle_label_text { std::format("Angle: {:.2f}", _angle) };

    // Using stringstream to format the string with fixed precision
    /*std::string angle_label_text{};
    std::stringstream stream{};
    stream << std::fixed << "Angle: " << std::setprecision(2) << _angle;
    angle_label_text = stream.str();

    TextRenderer.Label(angle_label_text.c_str(), _width / 2, _height - 4, SimpleText::CENTER);*/

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
        if ((posMapX >= -0.5f && posMapX <= 0.5f) && (posMapY >= -0.5f && posMapY <= 0.5f))
        {
            std::cout << "Dans la map" << std::endl;
        } else {
            std::cout << "Hors de la map" << std::endl;
        }

    }
}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/)
{
}

void App::cursor_position_callback(double /*xpos*/, double /*ypos*/)
{
}

void App::size_callback(int width, int height)
{
    _width = width;
    _height = height;

    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, _width, _height);

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