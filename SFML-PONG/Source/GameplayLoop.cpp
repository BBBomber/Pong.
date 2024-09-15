#include "../Include/GameplayLoop.h"
#include "../Include/AudioManager.h"
#include "../Include/GameStates/MainMenuState.h"
#include<iostream>

// Constructor: Initializes the window and sets up the game
GameplayLoop::GameplayLoop()
    : window(sf::VideoMode(800, 600), "Pong"),
    view(sf::FloatRect(0, 0, 800, 600)), // Initialize view with the window size
    aspectRatio(4.f / 3.f),               // 4:3 aspect ratio
    currentState(nullptr)  // No state initially set
{
    window.setView(view);              // Set the view
    window.setFramerateLimit(60);
}

// Destructor: Cleanup
GameplayLoop::~GameplayLoop()
{
    delete currentState;  // Clean up the current state
}


void GameplayLoop::initialize()
{
    setState(new MainMenuState(this));

    // Initialize the AudioManager at the start of the game
    AudioManager::getInstance().initialize();
}

// The main game loop
void GameplayLoop::run()
{

    // Initialize the current state before the game loop
    if (currentState)
        currentState->initialize();

    AudioManager::getInstance().playBackgroundMusic();

    while (window.isOpen())
    {
        // Calculate deltaTime once per frame
        float deltaTime = clock.restart().asSeconds();

        processEvents(deltaTime);
        update(deltaTime);
        render();

        performStateChange();
    }
}

// Queue the state change (deferred)
void GameplayLoop::queueStateChange(GameState* nextState)
{
    this->nextState = nextState;  // Store the next state, but don't switch yet
}

void GameplayLoop::setState(GameState* state)
{
    currentState = state;
    nextState = nullptr;
}

// Perform the actual state change
void GameplayLoop::performStateChange()
{
    if (nextState)  // If a state change has been queued
    {
        if (currentState)
        {
            delete currentState;  // Clean up the current state
        }
        currentState = nextState;  // Switch to the new state
        currentState->initialize();  // Initialize the new state
        nextState = nullptr;  // Reset the next state pointer
    }
}

// Handles input and events
void GameplayLoop::processEvents(float deltaTime)
{

    sf::Event event;

    // Delegate input to the current state after checking for events
    if (currentState)
    {
        currentState->handleInput(window, deltaTime);  // Handle state-specific input
    }

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::Resized)
        {
            maintainAspectRatio(); // Adjust the view when the window is resized
        }

        currentState->handleEventInput(event, window);
    }
}

// Update loop
void GameplayLoop::update(float deltaTime)
{
    if (currentState)
    {
        currentState->update(window, deltaTime);  // Delegate update to the current state
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

sf::RenderWindow& GameplayLoop::getWindow()
{
    return window;  // Provide access to the window for game states
}