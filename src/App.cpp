#include "App.hpp"
#include "itdReader.hpp"
#include "map.hpp"

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

App::App() : _previousTime(0.0), _viewSize(2.0)
{
    // load what needs to be loaded here (for example textures)
    // img::Image baseMap{img::load(make_absolute_path("images/map2.png", true), 3, true)};
    //_texture = loadTexture(baseMap);
    img::Image grassTile1{img::load(make_absolute_path("images/grass-tiles/grass-tile-1.png", true), 3, true)};
    // Création d'une texture
    _texture = loadTexture(grassTile1);
}

void App::setup()
{
    std::cout << "Launching map" << std::endl;
    img::Image baseMap{img::load(make_absolute_path("images/mapRGB.png", true), 3, true)};
    std::vector<std::vector<int>> nodes = checkMap();
    checkImage(baseMap);
    getNodes(nodes);
    Map map = compareMapItd(getNodes(nodes), checkImage(baseMap));

    // Chargement d'une image
    int x;
    int y;
    int n;
    std::string path = make_absolute_path("images/grass-tiles/grass-tile-1.png", true);
    unsigned char *image = stbi_load(path.c_str(), &x, &y, &n, 0);

    if (image == NULL)
    {
        std::cout << "Image not loaded" << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "Image loaded" << std::endl;
    }

    // Texture
    GLuint texture;
    glGenTextures(1, &texture);
    // On attache la texture
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);
    // On libère l'espace
    stbi_image_free(image);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    // on créé un quad pour afficher la texture
    // ici
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Set the clear color to a nice blue
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
    // Setup the text renderer with blending enabled and white text color
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


    TextRenderer.Label("Example of using SimpleText library", _width / 2, 20, SimpleText::CENTER);

    // Without set precision
    // const std::string angle_label_text { "Angle: " + std::to_string(_angle) };
    // With c++20 you can use std::format
    // const std::string angle_label_text { std::format("Angle: {:.2f}", _angle) };

    // Using stringstream to format the string with fixed precision
    std::string angle_label_text{};
    std::stringstream stream{};
    stream << std::fixed << "Angle: " << std::setprecision(2) << _angle;
    angle_label_text = stream.str();

    TextRenderer.Label(angle_label_text.c_str(), _width / 2, _height - 4, SimpleText::CENTER);

    TextRenderer.Render();
}

void App::key_callback(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/)
{
}

void App::mouse_button_callback(int /*button*/, int /*action*/, int /*mods*/)
{
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