#include <SFML/Graphics.hpp>

// Function to create and return a rectangle shape
sf::RectangleShape createRectangle(float width, float height, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColor, float outlineThickness)
{
    // Create a rectangle shape with the specified dimensions
    sf::RectangleShape rectangle(sf::Vector2f(width, height));

    // Set the position of the rectangle
    rectangle.setPosition(position);

    // Set the color of the rectangle (fill color)
    rectangle.setFillColor(fillColor);

    // Set the outline color and thickness
    rectangle.setOutlineColor(outlineColor);
    rectangle.setOutlineThickness(outlineThickness);

    return rectangle;
}

int main()
{
    // Create a window with dimensions 800x600 and title "Pong"
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");

    // Create a rectangle with 100x50 size, position it at (350, 275), set fill color, outline color, and thickness
    sf::RectangleShape rectangle = createRectangle(100, 50, sf::Vector2f(350, 275), sf::Color::Red, sf::Color::Yellow, 5.f);

    // Main game loop: runs as long as the window is open
    while (window.isOpen())
    {
        // Event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            // If the close button is pressed, close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window with a black color
        window.clear(sf::Color::Black);

        // Draw the rectangle with the outline
        window.draw(rectangle);

        // Display what has been drawn to the window
        window.display();
    }

    return 0;
}
