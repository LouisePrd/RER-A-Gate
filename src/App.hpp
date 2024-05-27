#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include "utils.hpp"
#include "itdReader.hpp"

class App {
public:
    App();

    void setup();
    void update();
    std::map<GLuint, typeCase> mappingTexture();
    
    // GLFW callbacks binding
    void key_callback(int key, int scancode, int action, int mods);
    void mouse_button_callback(int button, int action, int mods);
    void scroll_callback(double xoffset, double yoffset);
    void cursor_position_callback(double xpos, double ypos);
    void size_callback(int width, int height);

private:
    void render();

    int _width {};
    int _height {};
    double _previousTime {};
    float _viewSize {};

    // Add your variables here
    GLuint _texture {};
    float _angle {};
    std::vector<GLuint> _textures;
    SimpleText TextRenderer {};
};