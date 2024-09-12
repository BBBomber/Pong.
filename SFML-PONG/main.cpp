#include <SFML/Graphics.hpp>

// Function to create and return a rectangle shape
sf::RectangleShape createRectangle(float width, float height, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColor, float outlineThickness)
{
    sf::RectangleShape rectangle(sf::Vector2f(width, height));
    rectangle.setPosition(position);
    rectangle.setFillColor(fillColor);
    rectangle.setOutlineColor(outlineColor);
    rectangle.setOutlineThickness(outlineThickness);

    return rectangle;
}

// Function to create and return a circle shape (default color is white)
sf::CircleShape createCircle(float radius, sf::Vector2f position)
{
    sf::CircleShape circle(radius);
    circle.setPosition(position);

    return circle;
}

// Function to maintain the aspect ratio when the window is resized
void maintainAspectRatio(sf::RenderWindow& window, sf::View& view, float aspectRatio)
{
    sf::Vector2u windowSize = window.getSize();
    float windowAspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

    if (windowAspectRatio > aspectRatio) 
    {
        float newWidth = aspectRatio / windowAspectRatio;
        view.setViewport(sf::FloatRect((1.f - newWidth) / 2.f, 0.f, newWidth, 1.f));
    }
    else 
    {
        float newHeight = windowAspectRatio / aspectRatio;
        view.setViewport(sf::FloatRect(0.f, (1.f - newHeight) / 2.f, 1.f, newHeight));
    }

    window.setView(view);
}

// Function to move the ball based on velocity and delta time
void moveBall(sf::CircleShape& circle, sf::Vector2f& velocity, float deltaTime)
{
    circle.move(velocity * deltaTime);
}

// Function to handle ball collisions with the viewport boundaries (not the entire window)
void handleBallViewportCollision(sf::CircleShape& circle, sf::Vector2f& velocity, const sf::View& view)
{
    sf::Vector2f position = circle.getPosition();
    float radius = circle.getRadius();

    // Get the boundaries of the viewport (view's center and size)
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    // Calculate the edges of the viewport (where the game content is visible)
    float leftEdge = viewCenter.x - viewSize.x / 2.f;
    float rightEdge = viewCenter.x + viewSize.x / 2.f;
    float topEdge = viewCenter.y - viewSize.y / 2.f;
    float bottomEdge = viewCenter.y + viewSize.y / 2.f;

    // Check for collision with the viewport boundaries and reverse velocity
    if (position.x <= leftEdge) 
    {
        velocity.x = std::abs(velocity.x);  // Move right
        circle.setPosition(leftEdge, position.y);  // Nudge to prevent sticking
    }
    if (position.x + 2 * radius >= rightEdge) 
    {
        velocity.x = -std::abs(velocity.x);  // Move left
        circle.setPosition(rightEdge - 2 * radius, position.y);  // Nudge to prevent sticking
    }
    if (position.y <= topEdge) 
    {
        velocity.y = std::abs(velocity.y);  // Move down
        circle.setPosition(position.x, topEdge);  // Nudge to prevent sticking
    }
    if (position.y + 2 * radius >= bottomEdge) 
    {
        velocity.y = -std::abs(velocity.y);  // Move up
        circle.setPosition(position.x, bottomEdge - 2 * radius);  // Nudge to prevent sticking
    }
}

// Function to handle ball collisions with the rectangle
void handleBallRectangleCollision(sf::CircleShape& circle, sf::Vector2f& velocity, const sf::RectangleShape& rectangle)
{
    sf::FloatRect ballBounds = circle.getGlobalBounds();
    sf::FloatRect rectBounds = rectangle.getGlobalBounds();

    // Check if the ball intersects with the rectangle
    if (ballBounds.intersects(rectBounds))
    {
        // Simple collision response: reverse the velocity based on the side of impact
        if (ballBounds.left < rectBounds.left || ballBounds.left + ballBounds.width > rectBounds.left + rectBounds.width)
        {
            velocity.x = -velocity.x; // Reverse horizontal velocity
        }
        if (ballBounds.top < rectBounds.top || ballBounds.top + ballBounds.height > rectBounds.top + rectBounds.height)
        {
            velocity.y = -velocity.y; // Reverse vertical velocity
        }

        // Nudge the ball slightly out of the rectangle to prevent sticking
        if (ballBounds.left < rectBounds.left) 
        {
            circle.setPosition(rectBounds.left - ballBounds.width, ballBounds.top);
        }
        else if (ballBounds.left + ballBounds.width > rectBounds.left + rectBounds.width) 
        {
            circle.setPosition(rectBounds.left + rectBounds.width, ballBounds.top);
        }
        if (ballBounds.top < rectBounds.top) 
        {
            circle.setPosition(ballBounds.left, rectBounds.top - ballBounds.height);
        }
        else if (ballBounds.top + ballBounds.height > rectBounds.top + rectBounds.height) 
        {
            circle.setPosition(ballBounds.left, rectBounds.top + rectBounds.height);
        }
    }
}

// Function to calculate delta time for each frame
float getDeltaTime(sf::Clock& clock)
{
    return clock.restart().asSeconds();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");

    // Create a rectangle with 100x50 size, position it at (700, 275)
    sf::RectangleShape rectangle = createRectangle(100, 50, sf::Vector2f(700, 275), sf::Color::Red, sf::Color::Green, 5.f);

    // Create a white circle with radius 10, positioned at (200, 300)
    sf::CircleShape circle = createCircle(10, sf::Vector2f(200, 300));

    // Ball's initial velocity (moving diagonally)
    sf::Vector2f ballVelocity(300.f, 300.f);  // Speed in pixels per second

    // Create a view with the same size as the original window
    sf::View view(sf::FloatRect(0, 0, 800, 600));

    // Aspect ratio of 800x600 is 4:3
    float aspectRatio = 4.f / 3.f;

    // Clock for managing frame times
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle window resizing
            if (event.type == sf::Event::Resized)
            {
                maintainAspectRatio(window, view, aspectRatio);
            }
        }

        // Calculate delta time
        float deltaTime = getDeltaTime(clock);

        // Move the ball
        moveBall(circle, ballVelocity, deltaTime);

        // Handle ball collisions with viewport boundaries (not the entire window)
        handleBallViewportCollision(circle, ballVelocity, view);

        // Handle ball collisions with the rectangle
        handleBallRectangleCollision(circle, ballVelocity, rectangle);

        window.clear(sf::Color::Black);

        // Draw the rectangle and the moving ball
        window.draw(rectangle);
        window.draw(circle);

        window.display();
    }

    return 0;
}
