#include "../../Include/GameStates/MultiplayerGameplayState.h"
#include "../../Include/GameStates/GameOverState.h"
#include "../../Include/AudioManager.h"
#include "../../Include/AssetPaths.h"
#include <sstream>
#include <iostream>

MultiplayerGameplayState::MultiplayerGameplayState(GameplayLoop* loop)
    : gameLoop(loop), leftPaddle(sf::Vector2f(0, 250), sf::Keyboard::W, sf::Keyboard::S),
    rightPaddle(sf::Vector2f(790, 250), sf::Keyboard::Up, sf::Keyboard::Down), ball(10.f, 700.f),
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
    timeSinceLastUpdate += deltaTime;
    if (timeSinceLastUpdate >= timeBetweenUpdates)
    {
        if (isHost)
        {
            // Host sends its paddle position to the client
            std::string data = "PADDLE_LEFT:" + std::to_string(leftPaddle.getPosition().y);
            networkManager->sendDataToClient(data);

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
            // Client sends its paddle position to the host
            std::string data = "PADDLE_RIGHT:" + std::to_string(rightPaddle.getPosition().y);
            networkManager->sendDataToServer(data);

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

void MultiplayerGameplayState::syncBall(float deltaTime)
{
    if (isHost)
    {
        // Host sends ball position and velocity to the client
        std::string ballData = "BALL:" + std::to_string(ball.getPosition().x) + ":" +
            std::to_string(ball.getPosition().y) + ":" +
            std::to_string(ball.getVelocity().x) + ":" +
            std::to_string(ball.getVelocity().y);
        networkManager->sendDataToClient(ballData);

    }
    else
    {
        // Client receives ball data from the host
        std::string receivedBallData = networkManager->receiveDataFromServer();
        if (!receivedBallData.empty() && receivedBallData.find("BALL:") != std::string::npos)
        {
            std::stringstream ss(receivedBallData.substr(5));  // Skip "BALL:"
            float ballX, ballY, velX, velY;
            char delimiter;
            if (ss >> ballX >> delimiter >> ballY >> delimiter >> velX >> delimiter >> velY)
            {
                ball.setPosition(sf::Vector2f(ballX, ballY));
                ball.setVelocity(sf::Vector2f(velX, velY));

            }
            else
            {
                
            }
        }
    }
}

void MultiplayerGameplayState::syncScore()
{


    if (isHost)
    {
        static int lastSentLeftScore = -1, lastSentRightScore = -1;
        if (leftScore != lastSentLeftScore || rightScore != lastSentRightScore)
        {
            std::string scoreData = "SCORE:" + std::to_string(leftScore) + ":" + std::to_string(rightScore);
            networkManager->sendDataToClient(scoreData);
            std::cout << "Sent score to client: " << scoreData << std::endl;
            lastSentLeftScore = leftScore;
            lastSentRightScore = rightScore;
        }
    }
    else
    {
        std::string receivedScoreData = networkManager->receiveDataFromServer();

        if (!receivedScoreData.empty())
        {
            std::cout << "Client received data: " << receivedScoreData << std::endl;

            size_t scorePos = receivedScoreData.find("SCORE:");
            if (scorePos != std::string::npos)
            {
                std::istringstream iss(receivedScoreData.substr(scorePos + 6));
                int updatedLeftScore, updatedRightScore;
                char delimiter;
                if (iss >> updatedLeftScore >> delimiter >> updatedRightScore)
                {
                    if (leftScore != updatedLeftScore || rightScore != updatedRightScore)
                    {
                        leftScore = updatedLeftScore;
                        rightScore = updatedRightScore;
                        updateScoreText();
                        std::cout << "Client updated score: Left=" << leftScore << ", Right=" << rightScore << std::endl;
                    }
                    else
                    {
                        std::cout << "Received score update, but scores haven't changed" << std::endl;
                    }
                }
                else
                {
                    std::cerr << "Error parsing score data on client: " << receivedScoreData << std::endl;
                }
            }
        }
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
            std::to_string(leftScore) + ":" + std::to_string(rightScore);

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
                        ball.setPosition(sf::Vector2f(ballX, ballY));
                        ball.setVelocity(sf::Vector2f(velX, velY));
                        std::cerr << "Ball position updated: (" << ballX << ", " << ballY << ")" << std::endl;

                        ball.update(deltaTime, window);
                    }
                    else
                    {
                        std::cerr << "Error parsing ball data: " << ballData << std::endl;
                    }
                }

                // Parse score data
                if (scoreData.find("SCORE:") == 0)
                {
                    std::stringstream ss(scoreData.substr(6));  // Skip "SCORE:"
                    int updatedLeftScore, updatedRightScore;
                    char delimiter;
                    if (ss >> updatedLeftScore >> delimiter >> updatedRightScore)
                    {
                        // Validate the score before updating
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
