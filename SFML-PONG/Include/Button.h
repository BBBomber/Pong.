#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class Button {
public:
    Button(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size = { 200.0f, 50.0f },
        const sf::Color& buttonColor = sf::Color::Blue, const sf::Color& textColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Black, float borderThickness = 2.0f);

    // Set the function to be called when the button is clicked
    void setOnClick(void (*callback)());

    void render(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

private:
    sf::Text buttonText;
    sf::RectangleShape buttonShape;
    sf::Font buttonFont;

    void (*onClick)() = nullptr;  // Callback for button click

    void loadFont();

    // To init everything
    void init(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size,
        const sf::Color& buttonColor, const sf::Color& textColor, const sf::Color& borderColor,
        float borderThickness);

};


