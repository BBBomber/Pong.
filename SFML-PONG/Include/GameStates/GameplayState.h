#pragma once

#include "../GameState.h"
#include "../GameplayElements/Paddle.h"
#include "../GameplayElements/Ball.h"

class GameplayState : public GameState
{
public:
    GameplayState();               // Constructor
    ~GameplayState();              // Destructor

    void initialize() override;    // Initialize the gameplay
    void handleInput(sf::RenderWindow& window, float deltaTime) override;  // Handle input for gameplay
    void update(sf::RenderWindow& window, float deltaTime) override;  // Update the gameplay state
    void render(sf::RenderWindow& window) override;  // Render the gameplay

private:
    Paddle leftPaddle;            // Left paddle (W and S)
    Paddle rightPaddle;           // Right paddle (Up and Down arrow keys)
    Ball ball;                    // ball lol
};