#include "../Include/GameplayLoop.h"

// Constructor: Initializes the window and sets up the game
GameplayLoop::GameplayLoop()
    : window(sf::VideoMode(800, 600), "Pong"),
    view(sf::FloatRect(0, 0, 800, 600)), // Initialize view with the window size
    aspectRatio(4.f / 3.f)               // 4:3 aspect ratio
{
    window.setView(view);              // Set the view
}

// Destructor: Cleanup
GameplayLoop::~GameplayLoop()
{
}

// The main game loop
void GameplayLoop::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

// Handles input and events
void GameplayLoop::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::Resized)
        {
            maintainAspectRatio(); // Adjust the view when the window is resized
        }
    }
}

// Update loop
void GameplayLoop::update()
{
    // Game logic goes here (for now, it's empty)
}

// Renders everything to the screen
void GameplayLoop::render()
{
    window.clear(sf::Color::Black);

    // Drawing code goes here

    window.display();
}

// Maintains the aspect ratio of the view
void GameplayLoop::maintainAspectRatio()
{
    sf::Vector2u windowSize = window.getSize();
    float windowAspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

    if (windowAspectRatio > aspectRatio) {
        // Window is wider than the desired aspect ratio, adjust the width
        float newWidth = aspectRatio / windowAspectRatio;
        view.setViewport(sf::FloatRect((1.f - newWidth) / 2.f, 0.f, newWidth, 1.f));
    }
    else {
        // Window is taller than the desired aspect ratio, adjust the height
        float newHeight = windowAspectRatio / aspectRatio;
        view.setViewport(sf::FloatRect(0.f, (1.f - newHeight) / 2.f, 1.f, newHeight));
    }

    window.setView(view); // Apply the updated view to the window
}
