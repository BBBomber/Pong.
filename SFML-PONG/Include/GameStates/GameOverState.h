#pragma once

#include "../GameState.h"
#include "../Button.h"
#include "../GameplayLoop.h"
#include <SFML/Graphics.hpp>

class GameOverState : public GameState
{
public:
    GameOverState(GameplayLoop* loop, const std::string& winner);
    ~GameOverState() = default;

    void initialize() override;
    void handleInput(sf::RenderWindow& window, float deltaTime) override;
    void handleEventInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window, float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    GameplayLoop* gameLoop;
    std::string winner;

    sf::Font font;
    sf::Text gameOverText;
    Button mainMenuButton;
    Button quitButton;

    void initButtons();
    void onMainMenuButtonClick();
    void onQuitButtonClick();
};