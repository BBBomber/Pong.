#pragma once

#include <SFML/Graphics.hpp>

class GameState
{
public:
    virtual ~GameState() = default;

    // Initializes the state (e.g., load resources, set up UI)
    virtual void initialize() = 0;

    // Handles input for this state
    virtual void handleInput(sf::RenderWindow& window, float deltaTime) = 0;

    // Updates the state logic
    virtual void update(sf::RenderWindow& window, float deltaTime) = 0;

    // Renders the state to the screen
    virtual void render(sf::RenderWindow& window) = 0;
};