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
bool App::isHovering(float x, float y, float width, float height)
{
    double xpos, ypos;
    int windowWidth, windowHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &windowWidth, &windowHeight);
    glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);

    const float aspectRatio = static_cast<float>(windowWidth) / windowHeight;
    float posMapX = (static_cast<float>(xpos) / windowWidth - (sizeDisplay / 2.f)) * _viewSize * aspectRatio;
    float posMapY = ((sizeDisplay / 2.f) - static_cast<float>(ypos) / windowHeight) * _viewSize;

    return (posMapX >= x && posMapX <= x + width && posMapY >= y && posMapY <= y + height);
}

// ========== DISPLAYS ==========
void App::displayTowerButtons()
{
    float buttonX = -(sizeDisplay / 2.f) + 0.1f;
    float buttonY = -0.7f;
    int textureId = 0;
    for (int i = 0; i < 3; i++)
    {
        displayButton(buttonX, buttonY, textureId);
        buttonX += 0.15f;
        textureId++;
    }
}

void App::displayButton(float buttonX, float buttonY, int idTexture)
{
    float buttonSize = 0.1f;
    float buttonWidth = buttonSize;
    float buttonHeight = buttonSize;

    if (isHovering(buttonX, buttonY, buttonWidth, buttonHeight))
    {
        glfwSetCursor(glfwGetCurrentContext(), glfwCreateStandardCursor(GLFW_HAND_CURSOR));

        if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            selectedTowerType = idTexture;
        }
    }
    else
    {
        glfwSetCursor(glfwGetCurrentContext(), glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texturesTower[idTexture]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(buttonX, buttonY);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(buttonX + buttonSize, buttonY);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(buttonX + buttonSize, buttonY + buttonSize);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(buttonX, buttonY + buttonSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void App::displayMoney()
{
    float iconSize = 0.1f;
    float iconX = 0.6f;
    float iconY = 0.55f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texturesMap[7]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(iconX, iconY);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(iconX + iconSize, iconY);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(iconX + iconSize, iconY + iconSize);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(iconX, iconY + iconSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    std::string moneyText = std::to_string(totalMoney);
    const char *moneyTextChar = moneyText.c_str();
    float textX = (sizeDisplay/2 + 0.2f) * _width;
    float textY = 0.22f * _height;

    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
    TextRenderer.SetTextSize(SimpleText::FontSize::SIZE_48);
    TextRenderer.Label(moneyTextChar, textX, textY, SimpleText::LEFT);
}

void App::displayPrices()
{
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
    TextRenderer.SetTextSize(SimpleText::FontSize::SIZE_48);
    float buttonX = sizeDisplay / 2.f - 0.105f;
    float buttonY = sizeDisplay / 2.f + 0.3f;
    for (int i = 0; i < 3; i++)
    {
        std::string priceText = std::to_string(map.listPrices[i]);
        float textX = buttonX * _width;
        float textY = buttonY * _height;
        TextRenderer.Label(priceText.c_str(), textX, textY, SimpleText::CENTER);
        buttonX += 0.045f;
    }
}