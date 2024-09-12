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

// Function to maintain the aspect ratio when the window is resized
void maintainAspectRatio(sf::RenderWindow& window, sf::View& view, float aspectRatio)
{
    // Get the new window size
    sf::Vector2u windowSize = window.getSize();

    // Calculate the new aspect ratio
    float windowAspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

    // Determine if we need to add letterboxing (black bars) on the sides or top/bottom
    if (windowAspectRatio > aspectRatio) {
        // Window is wider than the desired aspect ratio, add black bars on the sides
        float newWidth = aspectRatio / windowAspectRatio;
        view.setViewport(sf::FloatRect((1.f - newWidth) / 2.f, 0.f, newWidth, 1.f));
    }
    else {
        // Window is taller than the desired aspect ratio
        float newHeight = windowAspectRatio / aspectRatio;
        view.setViewport(sf::FloatRect(0.f, (1.f - newHeight) / 2.f, 1.f, newHeight));
    }

    // Apply the view with the correct aspect ratio
    window.setView(view);
}

int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");

    //changed the x location to show the fullscreen maintained aspect ratio
    sf::RectangleShape rectangle = createRectangle(100, 50, sf::Vector2f(700, 275), sf::Color::Red, sf::Color::Green, 5.f);

    // Create a view with the same size as the original window
    sf::View view(sf::FloatRect(0, 0, 800, 600)); //think of this to be a sort of camera

    // Aspect ratio of 800x600 is 4:3
    float aspectRatio = 4.f / 3.f;

    while (window.isOpen())
    {
   
        sf::Event event;
        while (window.pollEvent(event))
        {
            // If the close button is pressed, close the window
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle window resizing
            if (event.type == sf::Event::Resized)
            {
                // Maintain the aspect ratio when the window is resized
                maintainAspectRatio(window, view, aspectRatio); //comment this and go fullscreen to see the difference
            }
        }

        window.clear(sf::Color::Black); 

        // Draw the rectangle with the correct aspect ratio
        window.draw(rectangle);
        window.display();
    }

    return 0;
}