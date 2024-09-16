// JoinLobbyState.h
#pragma once

#include "../GameState.h"
#include "../GameplayLoop.h"
#include "../Button.h"
#include <SFML/Graphics.hpp>
#include <string>

class NetworkManager;

class JoinLobbyState : public GameState
{
public:
    JoinLobbyState(GameplayLoop* loop);
    ~JoinLobbyState() = default;

    void initialize() override;
    void handleInput(sf::RenderWindow& window, float deltaTime) override;
    void handleEventInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window, float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text lobbyCodeText;  // Text for displaying the entered lobby code
    sf::Text promptText;     // Prompt for entering lobby code
    std::string lobbyCode;   // Stores the lobby code (IP address for local testing)

    Button joinButton;       // Button to join the game
    Button backButton;       // Button to go back to the main menu

    NetworkManager* networkManager;

    GameplayLoop* gameLoop;         // Reference to the gameplay loop

    void onJoinButtonClick();
    void onBackButtonClick();
};
