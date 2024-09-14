#include "../../Include/GameplayElements/Ball.h"



// Constructor to initialize ball's radius and speed
Ball::Ball(float radius, float speed)
    : radius(radius), speed(speed), velocity(-speed, -speed)
{
    ballShape.setRadius(radius);  // Set the radius of the ball
    ballShape.setFillColor(sf::Color::White);  // Set the ball's color
    ballShape.setOrigin(radius, radius);  // Set origin to center for easier positioning
}

// Initialize the ball's position and velocity
void Ball::initialize()
{
    // Start the ball in the center of the screen
    ballShape.setPosition(400, 300);  // Assuming a default 800x600 view
    velocity = sf::Vector2f(-speed, -speed);  // Initial velocity
}

// Update the ball's movement and check for window and paddle collisions
void Ball::update(float deltaTime, const sf::RenderWindow& window)
{
    // Move the ball based on velocity
    ballShape.move(velocity * deltaTime);

    // Check for collision with window boundaries (viewport boundaries)
    checkWindowCollision(window);
}

// Render the ball on the window
void Ball::render(sf::RenderWindow& window)
{
    window.draw(ballShape);
}

// Handle ball collision with paddles
void Ball::handleCollision(const Paddle& paddle)
{
    sf::FloatRect ballBounds = ballShape.getGlobalBounds();
    sf::FloatRect paddleBounds = paddle.getBounds();

    // Check if the ball collides with the paddle
    if (ballBounds.intersects(paddleBounds))
    {
        // Reverse the horizontal velocity (bounce back)
        velocity.x = -velocity.x;

        // Move the ball out of the paddle to prevent it from sticking
        if (velocity.x > 0)  // Moving right
        {
            ballShape.setPosition(paddleBounds.left + paddleBounds.width + radius, ballShape.getPosition().y);
        }
        else  // Moving left
        {
            ballShape.setPosition(paddleBounds.left - radius, ballShape.getPosition().y);
        }
    }
}

// Check for collision with the top/bottom of the window
void Ball::checkWindowCollision(const sf::RenderWindow& window)
{
    sf::FloatRect ballBounds = ballShape.getGlobalBounds();

    // Get the viewport size based on the current view
    sf::View view = window.getView();
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f viewCenter = view.getCenter();

    // Calculate the top and bottom boundaries of the viewport
    float viewTop = viewCenter.y - (viewSize.y / 2.f);
    float viewBottom = viewCenter.y + (viewSize.y / 2.f);

    // Check if the ball hits the top or bottom of the viewport
    if (ballBounds.top < viewTop)
    {
        // Hit the top: reverse vertical velocity and reposition
        velocity.y = -velocity.y;
        ballShape.setPosition(ballShape.getPosition().x, viewTop + radius);
    }
    else if (ballBounds.top + ballBounds.height > viewBottom)
    {
        // Hit the bottom: reverse vertical velocity and reposition
        velocity.y = -velocity.y;
        ballShape.setPosition(ballShape.getPosition().x, viewBottom - radius);
    }
}

// Reset the ball to the center after a goal
void Ball::reset()
{
    // Set the ball back to the center of the screen
    ballShape.setPosition(400, 300);  

    // Reverse the velocity to simulate ball moving in the opposite direction
    velocity.x = -velocity.x;
}

sf::FloatRect Ball::getBounds() const
{
    return ballShape.getGlobalBounds();
}

