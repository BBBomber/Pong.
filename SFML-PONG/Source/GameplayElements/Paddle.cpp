#include "../../Include/GameplayElements/Paddle.h"


// Constructor to initialize position and control keys
Paddle::Paddle(sf::Vector2f position, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey)
    : upKey(upKey), downKey(downKey)
{
    paddleShape.setSize(paddleSize); 
    paddleShape.setPosition(position);          

}

// Handle input: move up or down based on user input
void Paddle::handleInput(sf::RenderWindow& window, float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(upKey))
    {
        paddleShape.move(0, -speed * deltaTime); // Move up
    }
    else if (sf::Keyboard::isKeyPressed(downKey))
    {
        paddleShape.move(0, speed * deltaTime);  // Move down
    }
}

// Update paddle logic with boundary checks
void Paddle::update(float deltaTime, const sf::RenderWindow& window)
{
    // Get the current position of the paddle
    sf::FloatRect bounds = paddleShape.getGlobalBounds();

    // Get the size of the view (the actual game area, accounting for the aspect ratio)
    sf::View view = window.getView();
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f viewCenter = view.getCenter();

    // Calculate the top and bottom bounds based on the view's center and size
    float viewTop = viewCenter.y - (viewSize.y / 2.f);
    float viewBottom = viewCenter.y + (viewSize.y / 2.f);

    // Ensure paddle stays within the view bounds (not the window size)
    if (bounds.top < viewTop)  // Prevent going above the view
    {
        paddleShape.setPosition(bounds.left, viewTop);
    }
    else if (bounds.top + bounds.height > viewBottom)  // Prevent going below the view
    {
        paddleShape.setPosition(bounds.left, viewBottom - bounds.height);
    }
}

// Render the paddle on the window
void Paddle::render(sf::RenderWindow& window)
{
    window.draw(paddleShape);
}

// Get the bounds of the paddle for collision detection
sf::FloatRect Paddle::getBounds() const
{
    return paddleShape.getGlobalBounds();
}

sf::Vector2f Paddle::getPosition()
{
    return paddleShape.getPosition();
}

void Paddle::setPosition(sf::Vector2f pos)
{
    paddleShape.setPosition(pos);
}
