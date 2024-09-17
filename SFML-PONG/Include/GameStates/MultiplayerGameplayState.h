#pragma once


#include "../GameState.h"
#include "../GameplayLoop.h"
#include "../GameplayElements/Paddle.h"
#include "../GameplayElements/Ball.h"
#include "../NetworkManager.h"

class MultiplayerGameplayState : public GameState
{
public:
    MultiplayerGameplayState(GameplayLoop* loop);
    ~MultiplayerGameplayState();

    void initialize() override;
    void handleInput(sf::RenderWindow& window, float deltaTime) override;
    void handleEventInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window, float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    Paddle leftPaddle;
    Paddle rightPaddle;
    Ball ball;

    int leftScore;
    int rightScore;

    bool isHost;                // Indicates whether this player is the host
    NetworkManager* networkManager;    // Pointer to the network manager
    GameplayLoop* gameLoop;     // Reference to the gameplay loop

    sf::Font font;
    sf::Text leftScoreText;
    sf::Text rightScoreText;

    const float timeBetweenUpdates = 0.05f;  // 50 ms between updates
    float timeSinceLastUpdate = 0.0f;

    void checkGoal();
    void reset();
    void updateScoreText();
    void drawDashedLine(sf::RenderWindow& window);

    void syncPaddles(float deltaTime);
    void syncBall(float deltaTime);
    void syncScore();
    void syncBallAndScore(float deltaTime, sf::RenderWindow& window);
};

