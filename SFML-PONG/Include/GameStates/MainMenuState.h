#pragma once

#include "../GameState.h"
#include "../GameplayLoop.h"
#include "../Button.h"
#include <SFML/Graphics.hpp>

class MainMenuState : public GameState
{
public:
    MainMenuState(GameplayLoop* loop);  // Accept the loop reference
    ~MainMenuState() = default;

    void initialize() override;
    void handleInput(sf::RenderWindow& window, float deltaTime) override;
    void handleEventInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::RenderWindow& window, float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Text titleText;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    Button playButton;
    Button quitButton;
    Button HostGame;

    GameplayLoop* gameLoop;  // Store the reference to the gameplay loop

    void onPlayButtonClick();
    void onQuitButtonClick();
    void onHostButtonClick();

    void initButtons();
    void initBackground();
    void initTitle();
};