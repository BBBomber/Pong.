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
        // Host updates the ball and sends its data to the client
        ball.update(deltaTime, window);
        syncBall(deltaTime);
    }

    ball.handleCollision(leftPaddle);
    ball.handleCollision(rightPaddle);

    checkGoal();
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
    // Similar to single-player, but update scores and sync them between host and client
    sf::Vector2f ballPos = ball.getBounds().getPosition();

    if (ballPos.x < 0)
    {
        rightScore++;
        updateScoreText();

        if (rightScore == 5)
        {
            // Trigger game over for right player
            gameLoop->queueStateChange(new GameOverState(gameLoop, "Right"));
        }
        else
        {
            reset();
        }
    }
    else if (ballPos.x > 800)
    {
        leftScore++;
        updateScoreText();

        if (leftScore == 5)
        {
            // Trigger game over for left player
            gameLoop->queueStateChange(new GameOverState(gameLoop, "Left"));
        }
        else
        {
            reset();
        }
    }
}

void MultiplayerGameplayState::syncPaddles(float deltaTime)
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
}

void MultiplayerGameplayState::syncBall(float deltaTime)
{
    if (isHost)
    {
        // Send ball position and velocity to client
        std::string ballData = "BALL:" + std::to_string(ball.getPosition().x) + ":" +
            std::to_string(ball.getPosition().y) + ":" +
            std::to_string(ball.getVelocity().x) + ":" +
            std::to_string(ball.getVelocity().y);
        networkManager->sendDataToClient(ballData);
    }
    else
    {
        // Receive ball data from host
        std::string ballData = networkManager->receiveDataFromServer();
        if (!ballData.empty() && ballData.find("BALL:") != std::string::npos)
        {
            std::stringstream ss(ballData.substr(5));
            float ballX, ballY, velX, velY;
            char delimiter;
            ss >> ballX >> delimiter >> ballY >> delimiter >> velX >> delimiter >> velY;

            ball.setPosition(sf::Vector2f(ballX, ballY));
            ball.setVelocity(sf::Vector2f(velX, velY));
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
