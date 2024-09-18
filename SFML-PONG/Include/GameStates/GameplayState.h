#pragma once

#include "../GameState.h"
#include "../GameplayLoop.h"

class GameplayState : public GameState
{
public:
    GameplayState(GameplayLoop* loop);               // Constructor
    ~GameplayState();              // Destructor

    void initialize() override;
    void handleInput(sf::RenderWindow& window, float deltaTime) override;
    void handleEventInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window, float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    // Empty for now, but later we'll add paddles, ball, score, etc.

    GameplayLoop* gameLoop;       // Reference to GameplayLoop for state management
};