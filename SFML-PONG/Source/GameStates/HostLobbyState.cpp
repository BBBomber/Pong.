// HostLobbyState.cpp

#include "../../Include/GameStates/HostLobbyState.h"
#include "../../Include/GameStates/GameplayState.h"
#include "../../Include/AssetPaths.h"
#include <iostream>

HostLobbyState::HostLobbyState(GameplayLoop* loop)
    : gameLoop(loop), clientJoined(false)
{
    // Generate a lobby code
    generateLobbyCode();
}

void HostLobbyState::initialize()
{
    if (!font.loadFromFile(AssetPaths::mainFont))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    // Setup lobby code display
    lobbyCodeText.setFont(font);
    lobbyCodeText.setString("Lobby Code: " + lobbyCode);
    lobbyCodeText.setCharacterSize(30);
    lobbyCodeText.setFillColor(sf::Color::White);
    lobbyCodeText.setPosition(250.0f, 200.0f);

    // Setup waiting for client text
    waitingText.setFont(font);
    waitingText.setString("Waiting for client to join...");
    waitingText.setCharacterSize(25);
    waitingText.setFillColor(sf::Color::White);
    waitingText.setPosition(250.0f, 300.0f);

    // Setup start game text (hidden for now)
    startGameText.setFont(font);
    startGameText.setString("Press Enter to Start Game");
    startGameText.setCharacterSize(25);
    startGameText.setFillColor(sf::Color::Green);
    startGameText.setPosition(250.0f, 400.0f);
}

void HostLobbyState::handleInput(sf::RenderWindow& window, float deltaTime)
{
    // Simulate client joining after 3 seconds for now
    simulateClientJoin();

    if (clientJoined)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            onStartGame(); // Start the game when Enter is pressed
        }
    }
}

void HostLobbyState::handleEventInput(const sf::Event& event, sf::RenderWindow& window)
{
    // Handle any specific event input for this state
}

void HostLobbyState::update(sf::RenderWindow& window, float deltaTime)
{
    // Nothing to update right now
}

void HostLobbyState::render(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    window.draw(lobbyCodeText);

    if (!clientJoined)
    {
        window.draw(waitingText);
    }
    else
    {
        window.draw(startGameText); // Show start game text if client joined
    }
}

void HostLobbyState::generateLobbyCode()
{
    // Simple random number as lobby code for now
    lobbyCode = std::to_string(rand() % 9000 + 1000); // Random 4-digit number
}

void HostLobbyState::simulateClientJoin()
{
    // For now, simulate the client joining after a delay (3 seconds)
    static float timeElapsed = 0.0f;
    timeElapsed += 1.0f;

    if (timeElapsed >= 3.0f)
    {
        clientJoined = true;
    }
}

void HostLobbyState::onStartGame()
{
    // Queue the transition to GameplayState
    gameLoop->queueStateChange(new GameplayState(gameLoop));
}
