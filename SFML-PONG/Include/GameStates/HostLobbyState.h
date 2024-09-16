// HostLobbyState.h

#pragma once

#include "../GameState.h"
#include "../GameplayLoop.h"
#include <SFML/Graphics.hpp>

class HostLobbyState : public GameState
{
public:
    HostLobbyState(GameplayLoop* loop);
    ~HostLobbyState() = default;

    void initialize() override;
    void handleInput(sf::RenderWindow& window, float deltaTime) override;
    void handleEventInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window, float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    GameplayLoop* gameLoop;
    std::string lobbyCode;
    bool clientJoined;

    sf::Text lobbyCodeText;
    sf::Text waitingText;
    sf::Text startGameText;
    sf::Font font;

    void generateLobbyCode();
    void simulateClientJoin(); // For now, we'll simulate the client joining
    void onStartGame();
};

