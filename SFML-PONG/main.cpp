#include <SFML/Graphics.hpp>

int main()
{
    // Create a window with dimensions 800x600 and title "Pong" (Default style: Titlebar, Resize, Close)
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong", sf::Style::Default);

    // Uncomment one of the following lines to create windows with different styles:

    // Create a window with no decorations (no title bar, no borders, no close button)
    // sf::RenderWindow window(sf::VideoMode(800, 600), "Pong", sf::Style::None);

    // Create a window with only a title bar and close button (no resizing)
    // sf::RenderWindow window(sf::VideoMode(800, 600), "Pong", sf::Style::Titlebar | sf::Style::Close);

    // Create a window that can be resized, with a title bar and close button
    // sf::RenderWindow window(sf::VideoMode(800, 600), "Pong", sf::Style::Resize | sf::Style::Close);

    // Create a fullscreen window (no decorations, fills the screen)
    // sf::RenderWindow window(sf::VideoMode(1920, 1080), "Pong", sf::Style::Fullscreen);

    // Create a borderless window that fills the screen (no title bar, but not true fullscreen)
    //sf::RenderWindow window(sf::VideoMode(1920, 1080), "Pong", sf::Style::None);

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