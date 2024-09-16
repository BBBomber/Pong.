#pragma once

#include <SFML/Graphics.hpp>

class Paddle
{
public:
    Paddle(sf::Vector2f position, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey);  // Constructor to initialize position and control keys

    void handleInput(sf::RenderWindow& window, float deltaTime);    // Handle input for paddle movement
    void update(float deltaTime, const sf::RenderWindow& window);   // Update paddle movement with boundary checks
    void render(sf::RenderWindow& window); // Draw the paddle on the window

    sf::FloatRect getBounds() const;      // Get the paddle's bounds for collision detection
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f pos);


private:
    sf::RectangleShape paddleShape;       // The shape representing the paddle
    float speed = 500;                          // Speed of the paddle
    sf::Keyboard::Key upKey;              // Key to move paddle up
    sf::Keyboard::Key downKey;            // Key to move paddle down
    sf::Vector2f paddleSize = sf::Vector2f(10, 60);
};