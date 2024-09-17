// JoinLobbyState.cpp
#include "../../Include/GameStates/JoinLobbyState.h"
#include "../../Include/GameStates/MultiplayerGameplayState.h"
#include "../../Include/GameStates/MainMenuState.h"
#include "../../Include/NetworkManager.h"
#include "../../Include/AssetPaths.h"
#include <iostream>

JoinLobbyState::JoinLobbyState(GameplayLoop* loop)
    : gameLoop(loop),
    joinButton("Join Game", { 300.0f, 300.0f }, { 200.0f, 50.0f }),
    backButton("Back", { 300.0f, 400.0f }, { 200.0f, 50.0f }),
    lobbyCode("")  // Empty lobby code to start with
{
}

void JoinLobbyState::initialize()
{
    networkManager = gameLoop->getNetworkManager();
    if (!font.loadFromFile(AssetPaths::mainFont))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    // Prompt for lobby code
    promptText.setFont(font);
    promptText.setString("Enter Lobby Code:");
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(250.0f, 150.0f);

    // Text for displaying entered lobby code
    lobbyCodeText.setFont(font);
    lobbyCodeText.setCharacterSize(24);
    lobbyCodeText.setFillColor(sf::Color::White);
    lobbyCodeText.setPosition(250.0f, 200.0f);

    // Initialize the buttons
    joinButton.setOnClick([this]() { onJoinButtonClick(); });
    backButton.setOnClick([this]() { onBackButtonClick(); });
}

void JoinLobbyState::handleInput(sf::RenderWindow& window, float deltaTime)
{
    
}

void JoinLobbyState::handleEventInput(const sf::Event& event, sf::RenderWindow& window)
{
    // Handle input when a key is pressed
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Num0) lobbyCode += "0";
        if (event.key.code == sf::Keyboard::Num1) lobbyCode += "1";
        if (event.key.code == sf::Keyboard::Num2) lobbyCode += "2";
        if (event.key.code == sf::Keyboard::Num3) lobbyCode += "3";
        if (event.key.code == sf::Keyboard::Num4) lobbyCode += "4";
        if (event.key.code == sf::Keyboard::Num5) lobbyCode += "5";
        if (event.key.code == sf::Keyboard::Num6) lobbyCode += "6";
        if (event.key.code == sf::Keyboard::Num7) lobbyCode += "7";
        if (event.key.code == sf::Keyboard::Num8) lobbyCode += "8";
        if (event.key.code == sf::Keyboard::Num9) lobbyCode += "9";
        if (event.key.code == sf::Keyboard::Period) lobbyCode += ".";  // Handle the '.' character for IP addresses

        // Update the display with the current lobby code
        lobbyCodeText.setString(lobbyCode);
    }
    // Handle button events
    joinButton.handleEvent(event, window);
    backButton.handleEvent(event, window);
}

void JoinLobbyState::update(sf::RenderWindow& window, float deltaTime)
{
    
    
        // Continuously check for a message from the server (host)
        std::string serverMessage = networkManager->receiveDataFromServer();

        // Process only if we get a message
        if (!serverMessage.empty())
        {
            std::cout << "Received message: " << serverMessage << std::endl;

            // If the server sends the "START" message, transition to the gameplay state
            if (serverMessage == "START")
            {
                std::cout << "Host started the game! Transitioning to GameplayState." << std::endl;
                gameLoop->queueStateChange(new MultiplayerGameplayState(gameLoop));
            }
            else
            {
                std::cerr << "Unexpected message received: " << serverMessage << std::endl;
            }
        }
    
    
}

void JoinLobbyState::render(sf::RenderWindow& window)
{
    // Render the prompt, lobby code, and buttons
    window.clear(sf::Color::Black);
    window.draw(promptText);
    window.draw(lobbyCodeText);
    joinButton.render(window);
    backButton.render(window);
}

void JoinLobbyState::onJoinButtonClick()
{
    std::cout << "Joining game with lobby code: " << lobbyCode << std::endl;

    // Attempt to connect to the host server (for now, we're using localhost for testing)
    if (networkManager->connectToServer(lobbyCode, 54000))  // Assuming the server runs on port 54000
    {
        std::cout << "Connected to server! Waiting for the host to start the game." << std::endl;
      
    }
    else
    {
        std::cerr << "Failed to connect to server." << std::endl;
    }
}

void JoinLobbyState::onBackButtonClick()
{
    // Go back to the main menu
    gameLoop->queueStateChange(new MainMenuState(gameLoop));
}
