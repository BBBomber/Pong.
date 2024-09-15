#include "../Include/Button.h"
#include "../Include/AssetPaths.h"
#include <iostream>


Button::Button(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size,
    const sf::Color& buttonColor, const sf::Color& textColor, const sf::Color& borderColor,
    float borderThickness)
{
    // Use single init function
    init(text, position, size, buttonColor, textColor, borderColor, borderThickness);
}

void Button::init(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size,
    const sf::Color& buttonColor, const sf::Color& textColor, const sf::Color& borderColor,
    float borderThickness)
{
    buttonShape.setSize(size);
    buttonShape.setFillColor(buttonColor);
    buttonShape.setPosition(position);
    buttonShape.setOutlineColor(borderColor);
    buttonShape.setOutlineThickness(borderThickness);

    loadFont();
    buttonText.setFont(buttonFont);
    buttonText.setString(text);
    buttonText.setCharacterSize(static_cast<unsigned int>(size.y / 2));  // Set character size relative to button height
    buttonText.setFillColor(textColor);
    buttonText.setPosition(
        position.x + (size.x / 2.0f) - (buttonText.getLocalBounds().width / 2.0f),
        position.y + (size.y / 2.0f) - (buttonText.getLocalBounds().height / 2.0f) - buttonText.getCharacterSize() / 4.0f
    );
}


void Button::loadFont()
{
    if (!buttonFont.loadFromFile(AssetPaths::mainFont))
    {
        std::cerr << "Failed to load font!" << std::endl;
        // Handle error
    }
}

// Set the callback function for the button click event
void Button::setOnClick(void (*callback)())
{
    onClick = callback;
}

void Button::render(sf::RenderWindow& window)
{
    window.draw(buttonShape);
    window.draw(buttonText);
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

        if (buttonShape.getGlobalBounds().contains(worldPos) && onClick != nullptr)
        {
            onClick();  // Trigger the button's action
        }
    }
}