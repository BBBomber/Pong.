#pragma once

#include "../GameState.h"
#include "../GameplayLoop.h"
#include "../GameplayElements/Paddle.h"
#include "../GameplayElements/Ball.h"

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
    Paddle leftPaddle;            // Left paddle (W and S)
    Paddle rightPaddle;           // Right paddle (Up and Down arrow keys)
    Ball ball;                    // ball lol

    int leftScore;
    int rightScore;

    sf::Font font;                // Font used for rendering the score
    sf::Text leftScoreText;       // Text object to display the left player's score
    sf::Text rightScoreText;      // Text object to display the right player's score

    GameplayLoop* gameLoop;       // Reference to GameplayLoop for state management

    void checkGoal();
    void reset();
    void updateScoreText();       

    void drawDashedLine(sf::RenderWindow& window);  // Draw a dashed line in the center

};