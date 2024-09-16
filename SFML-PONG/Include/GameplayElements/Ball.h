#pragma once

#include <SFML/Graphics.hpp>
#include "Paddle.h"

class Ball
{
public:
    Ball(float radius = 10.f, float speed = 700.f);  // Constructor to initialize ball with optional radius and speed

    void initialize();                               // Initialize the ball's position and velocity
    void update(float deltaTime, const sf::RenderWindow& window);   // Update ball movement with collision checks
    void render(sf::RenderWindow& window);           // Render the ball on the window

    void handleCollision(const Paddle& paddle);      // Handle collision with paddles
    void reset();                                    // Reset position
    sf::FloatRect getBounds() const;                 // Get the ball's bounds

private:
    sf::CircleShape ballShape;                       // Shape representing the ball
    sf::Vector2f velocity;                           // Ball velocity (speed and direction)
    float speed;                                     // Ball speed
    float radius;                                    // Ball radius

    void checkWindowCollision(const sf::RenderWindow& window); // Check collision with the top/bottom of the window
};
