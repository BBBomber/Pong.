#include "../Include/GameplayLoop.h"

// Constructor: Initializes the window and sets up the game
GameplayLoop::GameplayLoop()
    : window(sf::VideoMode(800, 600), "Pong"),
    view(sf::FloatRect(0, 0, 800, 600)), // Initialize view with the window size
    aspectRatio(4.f / 3.f),               // 4:3 aspect ratio
    currentState(nullptr)  // No state initially set
{
    window.setView(view);              // Set the view
}

// Destructor: Cleanup
GameplayLoop::~GameplayLoop()
{
    delete currentState;  // Clean up the current state
}

// The main game loop
void GameplayLoop::run()
{

    // Initialize the current state before the game loop
    if (currentState)
        currentState->initialize();

    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

// Set the current game state
void GameplayLoop::setState(GameState* state)
{
    if (currentState)
    {
        delete currentState;  // Clean up the previous state
    }
    currentState = state;
    currentState->initialize();  // Initialize the new state
}

// Handles input and events
void GameplayLoop::processEvents()
{

    if (currentState)
    {
        currentState->handleInput(window);  // Delegate input to the current state
    }

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
    if (currentState)
    {
        float deltaTime = clock.restart().asSeconds();
        currentState->update(deltaTime);  // Delegate update to the current state
    }
}

// Renders everything to the screen
void GameplayLoop::render()
{
    window.clear(sf::Color::Black);

    if (currentState)
    {
        currentState->render(window);  // Delegate rendering to the current state
    }

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
