#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include "utils.hpp"
#include "itdReader.hpp"
#include "map.hpp"
#include "enemy.hpp"
#include "node.hpp"

class App
{
public:
    App();

    void setup();
    void update();
    void mappingTexture();
    void displayMap(Map map);
    void displayElement(int idTexture, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
    void displayEnemy(int idTexture, Enemy enemy);
    std::pair<int, int> getEndPos();
    void displayButton();

    // GLFW callbacks binding
    void key_callback(int key, int scancode, int action, int mods);
    void mouse_button_callback(int button, int action, int mods);
    void scroll_callback(double xoffset, double yoffset);
    void cursor_position_callback(double xpos, double ypos);
    void size_callback(GLFWwindow* window, int width, int height);

private:
    void render();

    int _width{};
    int _height{};
    double _previousTime{};
    float _viewSize{};
    Map map{};

    // Add your variables here
    GLuint _texture{};
    float _angle{};
    std::vector<GLuint> _texturesMap;
    std::vector<GLuint> _texturesEnemy;
    std::vector<GLuint> _texturesTower;

    SimpleText TextRenderer{};
};