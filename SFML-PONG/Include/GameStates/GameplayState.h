#pragma once

#include "../GameState.h"

class GameplayState : public GameState
{
public:
    GameplayState();               // Constructor
    ~GameplayState();              // Destructor

    void initialize() override;    // Initialize the gameplay
    void handleInput(sf::RenderWindow& window) override;  // Handle input for gameplay
    void update(float deltaTime) override;  // Update the gameplay state
    void render(sf::RenderWindow& window) override;  // Render the gameplay

private:
    // Empty for now, but later we'll add paddles, ball, score, etc.
};