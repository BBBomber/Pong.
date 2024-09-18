#pragma once

#include <SFML/Graphics.hpp>

class GameplayLoop
{
public:
    GameplayLoop();              // Constructor to initialize the game
    ~GameplayLoop();             // Destructor to clean up resources

    void run();                  // Function to start the game loop
    void initialize();
    sf::RenderWindow& getWindow();

private:

    sf::RenderWindow window;     // The SFML window where the game will run
    sf::View view;               // The view for maintaining aspect ratio
    sf::Clock clock;             // A clock to handle frame timing
    float aspectRatio;           // Aspect ratio for the game

    void processEvents(float deltaTime);        // Handles input and events
    void update(float deltaTime);               // Updates the game state
    void render();               // Draws the game to the screen
    void maintainAspectRatio();  // Function to maintain the aspect ratio

};