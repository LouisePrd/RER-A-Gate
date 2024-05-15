#include "App.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include <sstream>
#include <fstream>
#include <iostream>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"

App::App() : _previousTime(0.0), _viewSize(2.0)
{
    // load what needs to be loaded here (for example textures)

    img::Image test{img::load(make_absolute_path("images/level.png", true), 3, true)};

    _texture = loadTexture(test);
}

void App::checkMap()
{
    std::ifstream mapItd(make_absolute_path("data/.itd", true));
    std::vector<std::string> keyWords = {"ITD", "map", "path", "in", "out", "graph", "node"};
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(mapItd, line)) // Sans les commentaires
    {
        if (line[0] != '#')
            lines.push_back(line);
    }

    std::vector<std::string> values;
    for (size_t i = 0; i < keyWords.size(); i++) // Vérifie fichier
    {
        if (lines[i].find(keyWords[i]) == std::string::npos) // Vérifie mots clés
        {
            std::cerr << "Erreur fichier pour : " << keyWords[i] << std::endl;
            exit(1);
        }

        if (keyWords[i] == "in" || keyWords[i] == "out" || keyWords[i] == "path")
        {
            std::string s = lines[i];
            std::string delimiter = " ";
            size_t pos = 0;
            while ((pos = s.find(" ")) != std::string::npos)
            {
                std::string token = s.substr(0, pos);
                s.erase(0, pos + delimiter.length());
                if (std::all_of(token.begin(), token.end(), ::isdigit))
                    values.push_back(token);
            }
            if (std::all_of(s.begin(), s.end(), ::isdigit))
                values.push_back(s);
        }
    }

    size_t nbValues = values.size();
    for (size_t i = 0; i < nbValues; i++) // Vérifie valeurs RGB
    {
        if (nbValues != 9 || std::stoi(values[i]) < 0 || std::stoi(values[i]) > 255)
        {
            std::cerr << "Erreur fichier pour : " << keyWords[i] << std::endl;
            exit(1);
        }
    }
    
    // Vérifie fichier image
    try {
        img::Image test{img::load(make_absolute_path("images/level.png", true))};
    } catch (const std::exception& e) {
        std::cerr << "Erreur fichier image" << std::endl;
        exit(1);
    }

    exit(0);
}

void App::setup()
{
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

    _angle += 10.0f * elapsedTime;
    // _angle = std::fmod(_angle, 360.0f);

    render();
}

void App::render()
{
    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // render exemple quad
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();

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
