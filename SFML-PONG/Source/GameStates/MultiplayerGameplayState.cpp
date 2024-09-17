#include "../../Include/GameStates/MultiplayerGameplayState.h"
#include "../../Include/GameStates/GameOverState.h"
#include "../../Include/GameplayLoop.h"
#include "../../Include/AudioManager.h"
#include "../../Include/AssetPaths.h"
#include <sstream>
#include <iostream>

MultiplayerGameplayState::MultiplayerGameplayState(GameplayLoop* loop)
    : gameLoop(loop), leftPaddle(sf::Vector2f(0, 250), sf::Keyboard::W, sf::Keyboard::S),
    rightPaddle(sf::Vector2f(790, 250), sf::Keyboard::Up, sf::Keyboard::Down), ball(10.f, 400.f),
    leftScore(0), rightScore(0)
{
}

MultiplayerGameplayState::~MultiplayerGameplayState()
{
}

void MultiplayerGameplayState::initialize()
{
    // Initialize network manager
    networkManager = gameLoop->getNetworkManager();  // Get the network manager from the gameplay loop
    isHost = networkManager->getIsHost(); // Determine if this player is the host

    // Initialize ball
    ball.initialize();

    // Load font and initialize score texts
    if (!font.loadFromFile(AssetPaths::mainFont))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    leftScoreText.setFont(font);
    leftScoreText.setCharacterSize(30);
    leftScoreText.setFillColor(sf::Color::White);
    leftScoreText.setPosition(200, 20);

    rightScoreText.setFont(font);
    rightScoreText.setCharacterSize(30);
    rightScoreText.setFillColor(sf::Color::White);
    rightScoreText.setPosition(600, 20);

    updateScoreText();
}

void MultiplayerGameplayState::handleInput(sf::RenderWindow& window, float deltaTime)
{
    // Host controls the left paddle, client controls the right
    if (isHost)
    {
        leftPaddle.handleInput(window, deltaTime);
    }
    else
    {
        rightPaddle.handleInput(window, deltaTime);
    }
}

void MultiplayerGameplayState::handleEventInput(const sf::Event& event, sf::RenderWindow& window)
{
    // Handle any necessary event input
}

void MultiplayerGameplayState::update(sf::RenderWindow& window, float deltaTime)
{
    // Sync paddle positions
    syncPaddles(deltaTime);

    if (isHost)
    {
        // Host updates the ball and handles goal checking
        ball.update(deltaTime, window);  // Host updates the ball
        ball.handleCollision(leftPaddle);
        ball.handleCollision(rightPaddle);

        checkGoal();  // Only the host checks for goals and updates the score

        // Sync ball and score after the update and collision check
        syncBallAndScore(deltaTime, window);
    }
    else
    {
        // Client only receives ball position and score from the host
        syncBallAndScore(deltaTime, window);
    }

    
}

void MultiplayerGameplayState::render(sf::RenderWindow& window)
{
    drawDashedLine(window);

    leftPaddle.render(window);
    rightPaddle.render(window);
    ball.render(window);

    window.draw(leftScoreText);
    window.draw(rightScoreText);
}

void MultiplayerGameplayState::checkGoal()
{
    sf::Vector2f ballPos = ball.getBounds().getPosition();

    if (ballPos.x < 0)
    {
        rightScore++;
        updateScoreText();  // Update the score on the host's side

        if (rightScore == 50)
        {
            gameLoop->queueStateChange(new GameOverState(gameLoop, "Right"));
        }
        else
        {
            reset();  // Reset the ball after a goal
        }
    }
    else if (ballPos.x > 800)
    {
        leftScore++;
        updateScoreText();  // Update the score on the host's side
   
        if (leftScore == 50)
        {
            gameLoop->queueStateChange(new GameOverState(gameLoop, "Left"));
        }
        else
        {
            reset();  // Reset the ball after a goal
        }
    }
}

void MultiplayerGameplayState::syncPaddles(float deltaTime)
{
    static float lastSentLeftPaddleY = -1.0f;
    static float lastSentRightPaddleY = -1.0f;
    const float positionThreshold = 1.0f;  // Minimum movement needed to trigger a network update

    timeSinceLastUpdate += deltaTime;
    if (timeSinceLastUpdate >= timeBetweenUpdates)
    {
        if (isHost)
        {
            // Only send if the paddle has moved more than the threshold
            if (std::abs(leftPaddle.getPosition().y - lastSentLeftPaddleY) > positionThreshold)
            {
                std::string data = "PADDLE_LEFT:" + std::to_string(leftPaddle.getPosition().y);
                networkManager->sendDataToClient(data);
                lastSentLeftPaddleY = leftPaddle.getPosition().y;
            }

            // Receive client's paddle position
            std::string clientData = networkManager->receiveDataFromClient();
            if (!clientData.empty() && clientData.find("PADDLE_RIGHT:") != std::string::npos)
            {
                float clientPaddleY = std::stof(clientData.substr(13));
                rightPaddle.setPosition(sf::Vector2f(rightPaddle.getPosition().x, clientPaddleY));
            }
        }
        else
        {
            // Client sends its position only if it has moved significantly
            if (std::abs(rightPaddle.getPosition().y - lastSentRightPaddleY) > positionThreshold)
            {
                std::string data = "PADDLE_RIGHT:" + std::to_string(rightPaddle.getPosition().y);
                networkManager->sendDataToServer(data);
                lastSentRightPaddleY = rightPaddle.getPosition().y;
            }

            // Receive host's paddle position
            std::string hostData = networkManager->receiveDataFromServer();
            if (!hostData.empty() && hostData.find("PADDLE_LEFT:") != std::string::npos)
            {
                float hostPaddleY = std::stof(hostData.substr(12));
                leftPaddle.setPosition(sf::Vector2f(leftPaddle.getPosition().x, hostPaddleY));
            }
        }
        // Reset the timer
        timeSinceLastUpdate = 0.0f;
    }
}


void MultiplayerGameplayState::syncBallAndScore(float deltaTime, sf::RenderWindow& window)
{
    if (isHost)
    {
        // Host sends ball position, velocity, and score to the client in one message
        std::string combinedData = "BALL:" + std::to_string(ball.getPosition().x) + ":" +
            std::to_string(ball.getPosition().y) + ":" +
            std::to_string(ball.getVelocity().x) + ":" +
            std::to_string(ball.getVelocity().y) + "|SCORE:" +
            std::to_string(leftScore) + ":" + std::to_string(rightScore) + ";";  // Add message delimiter

        networkManager->sendDataToClient(combinedData);
        std::cerr << "Host sent: " << combinedData << std::endl;  // Debug logging
    }
    else
    {
        // Client receives combined ball and score data from the host
        std::string receivedData = networkManager->receiveDataFromServer();
        if (!receivedData.empty())
        {
            std::cerr << "Client received: " << receivedData << std::endl;  // Debug logging

            // Split the message by '|' to separate ball and score data
            size_t separatorIndex = receivedData.find('|');
            if (separatorIndex != std::string::npos)
            {
                std::string ballData = receivedData.substr(0, separatorIndex);
                std::string scoreData = receivedData.substr(separatorIndex + 1);

                // Parse ball data
                if (ballData.find("BALL:") == 0)
                {
                    std::stringstream ss(ballData.substr(5));  // Skip "BALL:"
                    float ballX, ballY, velX, velY;
                    char delimiter;
                    if (ss >> ballX >> delimiter >> ballY >> delimiter >> velX >> delimiter >> velY)
                    {
                        if (!isBallNearCollision())
                        {
                            // If no collision is expected, interpolate the ball position
                            sf::Vector2f currentPosition = ball.getPosition();
                            sf::Vector2f targetPosition(ballX, ballY);
                            float interpolationSpeed = 10.0f;  // Adjust for smoothness
                            sf::Vector2f newPosition = lerp(currentPosition, targetPosition, deltaTime * interpolationSpeed);
                            ball.setPosition(newPosition);
                        }
                        else
                        {
                            // Set the ball's position directly when a collision is expected
                            ball.setPosition(sf::Vector2f(ballX, ballY));
                        }

                        ball.setVelocity(sf::Vector2f(velX, velY));
                        std::cerr << "Ball position updated: (" << ball.getPosition().x << ", " << ball.getPosition().y << ")" << std::endl;
                    }
                    else
                    {
                        std::cerr << "Error parsing ball data: " << ballData << std::endl;
                    }
                }

                // Parse score data (remains the same)
                if (scoreData.find("SCORE:") == 0)
                {
                    std::stringstream ss(scoreData.substr(6));  // Skip "SCORE:"
                    int updatedLeftScore, updatedRightScore;
                    char delimiter;
                    if (ss >> updatedLeftScore >> delimiter >> updatedRightScore)
                    {
                        if (updatedLeftScore >= 0 && updatedRightScore >= 0)
                        {
                            leftScore = updatedLeftScore;
                            rightScore = updatedRightScore;
                            updateScoreText();
                            std::cerr << "Scores updated: Left = " << leftScore << ", Right = " << rightScore << std::endl;
                        }
                        else
                        {
                            std::cerr << "Invalid score received: " << scoreData << std::endl;
                        }
                    }
                    else
                    {
                        std::cerr << "Error parsing score data: " << scoreData << std::endl;
                    }
                }
            }
            else
            {
                std::cerr << "Invalid message format, missing '|' separator: " << receivedData << std::endl;
            }
        }
    }
}


bool MultiplayerGameplayState::isBallNearCollision()
{
    
    
    const float collisionThreshold = 30.0f;  // Adjust this threshold based on ball and paddle size
    sf::Vector2f ballPos = ball.getBounds().getPosition();
    sf::FloatRect ballBounds = ball.getBounds();
    sf::FloatRect leftPaddleBounds = leftPaddle.getBounds();
    sf::FloatRect rightPaddleBounds = rightPaddle.getBounds();
    sf::Vector2f windowSize(gameLoop->getWindow().getSize());

    // Check if the ball is near the left paddle
    if (ballPos.x <= leftPaddleBounds.left + leftPaddleBounds.width + collisionThreshold &&
        ballBounds.intersects(leftPaddleBounds))
    {
        return true;  // Ball is near the left paddle
    }

    // Check if the ball is near the right paddle
    if (ballPos.x >= rightPaddleBounds.left - collisionThreshold &&
        ballBounds.intersects(rightPaddleBounds))
    {
        return true;  // Ball is near the right paddle
    }

    // Check if the ball is near the top or bottom wall
    if (ballPos.y <= collisionThreshold || ballPos.y >= windowSize.y - collisionThreshold)
    {
        return true;  // Ball is near the top or bottom wall
    }

    // No collision expected
    return false;
}

// Linear interpolation function
sf::Vector2f MultiplayerGameplayState::lerp(sf::Vector2f a, sf::Vector2f b, float t)
{
    return a + t * (b - a);
}


void MultiplayerGameplayState::reset()
{
    ball.reset();
}

void MultiplayerGameplayState::updateScoreText()
{
    leftScoreText.setString(std::to_string(leftScore));
    rightScoreText.setString(std::to_string(rightScore));
}

void MultiplayerGameplayState::drawDashedLine(sf::RenderWindow& window)
{
    const float dashHeight = 15.0f;
    const float dashWidth = 5.0f;
    const float gap = 10.0f;

    sf::View currentView = window.getView();
    sf::Vector2f viewSize = currentView.getSize();
    sf::Vector2f viewCenter = currentView.getCenter();

    const float numDashes = viewSize.y / (dashHeight + gap);

    for (int i = 0; i < numDashes; ++i)
    {
        sf::RectangleShape dash(sf::Vector2f(dashWidth, dashHeight));
        dash.setPosition(viewCenter.x - dashWidth / 2.0f, (i * (dashHeight + gap)) - viewSize.y / 2.0f + viewCenter.y);
        dash.setFillColor(sf::Color::White);

        window.draw(dash);
    }
}
