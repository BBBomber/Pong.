// HostLobbyState.cpp
#include "../../Include/GameStates/HostLobbyState.h"
#include "../../Include/GameStates/MultiplayerGameplayState.h"
#include "../../Include/GameStates/MainMenuState.h"
#include "../../Include/NetworkManager.h"
#include "../../Include/AssetPaths.h"
#include <iostream>

HostLobbyState::HostLobbyState(GameplayLoop* loop)
    : gameLoop(loop),
    startGameButton("Start Game", { 300.0f, 300.0f }, { 200.0f, 50.0f }),
    backButton("Back", { 300.0f, 400.0f }, { 200.0f, 50.0f }),
    lobbyCode("127.0.0.1")  // Default to localhost for testing
{
}

HostLobbyState::~HostLobbyState()
{
    
    networkManager->stopServer();
}

void HostLobbyState::initialize()
{
    networkManager = gameLoop->getNetworkManager();
    networkManager->setIsHost(true);

    if (!font.loadFromFile(AssetPaths::mainFont))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    // Text for displaying the lobby code (IP address)
    lobbyCodeText.setFont(font);
    lobbyCodeText.setString("Lobby Code: " + lobbyCode);
    lobbyCodeText.setCharacterSize(24);
    lobbyCodeText.setFillColor(sf::Color::White);
    lobbyCodeText.setPosition(250.0f, 150.0f);

    // Initialize the buttons
    startGameButton.setOnClick([this]() { onStartGameButtonClick(); });
    backButton.setOnClick([this]() { onBackButtonClick(); });

    // Start the server and listen for incoming connections
    if (networkManager->startServer(54000))  // Listening on port 54000
    {
        std::cout << "Server started. Waiting for client to connect..." << std::endl;
    }
    else
    {
        std::cerr << "Failed to start the server." << std::endl;
    }
}

void HostLobbyState::handleInput(sf::RenderWindow& window, float deltaTime)
{
    // No real-time input to handle in the lobby state
}

void HostLobbyState::handleEventInput(const sf::Event& event, sf::RenderWindow& window)
{
    // Handle button events
    startGameButton.handleEvent(event, window);
    backButton.handleEvent(event, window);
}

void HostLobbyState::update(sf::RenderWindow& window, float deltaTime)
{
    
    // Check if a client has connected
    if (networkManager->isClientConnected())
    {
        std::cout << "Client connected! Ready to start the game." << std::endl;
        lobbyCodeText.setString("Client connected. Ready to start!");
    }
}

void HostLobbyState::render(sf::RenderWindow& window)
{
    window.clear();
    window.draw(lobbyCodeText);
    startGameButton.render(window);
    backButton.render(window);
}

void HostLobbyState::onStartGameButtonClick()
{
    
    // Check if a client is connected before starting the game
    if (networkManager->isClientConnected())
    {
        std::cout << "Starting the game!" << std::endl;

        // Notify the client that the game is starting
        networkManager->sendDataToClient("START;");

        

        // Transition to the gameplay state for the host
        gameLoop->queueStateChange(new MultiplayerGameplayState(gameLoop));
    }
    else
    {
        std::cerr << "No client connected. Can't start the game." << std::endl;
    }
}

void HostLobbyState::onBackButtonClick()
{
    // Go back to the main menu
    networkManager->setIsHost(false);
    gameLoop->queueStateChange(new MainMenuState(gameLoop));
}