// HostLobbyState.h
#pragma once

#include "../GameState.h"
#include "../GameplayLoop.h"
#include "../Button.h"
#include <SFML/Graphics.hpp>

 
class NetworkManager;

class HostLobbyState : public GameState
{
public:
    HostLobbyState(GameplayLoop* loop);
    ~HostLobbyState() override;

    void initialize() override;
    void handleInput(sf::RenderWindow& window, float deltaTime) override;
    void handleEventInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window, float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text lobbyCodeText;  // Text to display the generated lobby code (IP address)
    std::string lobbyCode;   // Stores the generated lobby code

    Button startGameButton;  // Button to start the game once the client connects
    Button backButton;       // Button to go back to the main menu

    NetworkManager* networkManager;
    GameplayLoop* gameLoop;         // Reference to the gameplay loop

    void onStartGameButtonClick();
    void onBackButtonClick();
};