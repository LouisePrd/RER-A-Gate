#include "userInterface.hpp"
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
#include "itdReader.hpp"
#include <stb_image/stb_image.h>
#include <map>

// ========== INTERACTIONS ==========
bool App::isHovering(float x, float y, float width, float height) {
    double xpos, ypos;
    int windowWidth, windowHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &windowWidth, &windowHeight);
    glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
    const float aspectRatio{windowWidth / (float)windowHeight};
    float posMapX = (xpos / windowWidth - 0.5f) * _viewSize * aspectRatio;
    float posMapY = (0.5f - ypos / windowHeight) * _viewSize;
    return (posMapX >= x && posMapX <= x + width && posMapY >= y && posMapY <= y + height);
}

void App::handleButtonInteraction(float buttonX, float buttonY, float buttonWidth, float buttonHeight) {
    if (isHovering(buttonX, buttonY, buttonWidth, buttonHeight) && glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        std::cout << "okkkkk" << std::endl;
    }
}

// ========== DISPLAYS ==========
void App::displayTowerButtons() {
    float buttonX = -0.5f;
    int textureId = 7;
    for(int i = 0; i < 3; i++) {
        displayButton(buttonX, -0.63f, textureId);
        buttonX += 0.15f;
        textureId++;
    }
}

void App::displayButton(float buttonX, float buttonY, int idTexture) {
    float buttonSize = 0.1f;

    double xpos, ypos;
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
    const float aspectRatio{width / (float)height};
    float posMapX = (xpos / width - 0.5f) * _viewSize * aspectRatio;
    float posMapY = (0.5f - ypos / height) * _viewSize;
    bool isHovering = (posMapX >= buttonX && posMapX <= buttonX + buttonSize && posMapY >= buttonY && posMapY <= buttonY + buttonSize);

    if (isHovering) {
        glColor3ub(178, 178, 178);
        glfwSetCursor(glfwGetCurrentContext(), glfwCreateStandardCursor(GLFW_HAND_CURSOR));
    } else {
        glfwSetCursor(glfwGetCurrentContext(), glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
        glColor3ub(128, 128, 128);

    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texturesMap[idTexture]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(buttonX, buttonY);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(buttonX + buttonSize, buttonY);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(buttonX + buttonSize, buttonY + buttonSize);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(buttonX, buttonY + buttonSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    float buttonWidth = buttonSize;
    float buttonHeight = buttonSize;
    handleButtonInteraction(buttonX, buttonY, buttonWidth, buttonHeight);
}