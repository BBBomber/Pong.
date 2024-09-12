#include <SFML/Graphics.hpp>

int main()
{
    // Create a window with dimensions 800x600 and title "Pong"
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");

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

        // Display what has been drawn to the window
        window.display();
    }

    return 0;
}
