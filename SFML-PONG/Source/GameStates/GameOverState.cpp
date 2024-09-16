#include "../../Include/GameStates/GameOverState.h"
#include "../../Include/GameStates/MainMenuState.h"
#include "../../Include/AssetPaths.h"
#include <iostream>

GameOverState::GameOverState(GameplayLoop* loop, const std::string& winner)
    : gameLoop(loop), winner(winner)
{
}

void GameOverState::initialize()
{
    if (!font.loadFromFile(AssetPaths::mainFont))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    // Set up the game over text
    gameOverText.setFont(font);
    gameOverText.setString(winner + " Player Wins!");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setStyle(sf::Text::Bold);
    // Get the size of the text and the viewport (view)
    sf::FloatRect textBounds = gameOverText.getLocalBounds();
    sf::View currentView = gameLoop->getWindow().getView();
    sf::Vector2f viewSize = currentView.getSize();

    // Center the text horizontally based on the viewport width
    gameOverText.setPosition(
        (viewSize.x - textBounds.width) / 2.0f - textBounds.left,  // Center horizontally
        100.0f  // Fixed vertical position
    );

    // Initialize buttons
    initButtons();
}

void GameOverState::handleInput(sf::RenderWindow& window, float deltaTime)
{
    // No real-time input required for game over state
}

void GameOverState::handleEventInput(const sf::Event& event, sf::RenderWindow& window)
{
    // Handle button events
    mainMenuButton.handleEvent(event, window);
    quitButton.handleEvent(event, window);
}

void GameOverState::update(sf::RenderWindow& window, float deltaTime)
{
    // No update logic required
}

void GameOverState::render(sf::RenderWindow& window)
{
    window.clear();
    window.draw(gameOverText);
    mainMenuButton.render(window);
    quitButton.render(window);
}

void GameOverState::initButtons()
{

    // Local variables for button parameters
    sf::Vector2f mainMenuButtonPos(300.0f, 300.0f);
    sf::Vector2f quitButtonPos(300.0f, 400.0f);
    sf::Vector2f buttonSize(200.0f, 50.0f);
    sf::Color mainMenuButtonColor(sf::Color::Green);
    sf::Color quitButtonColor(sf::Color::Red);
    sf::Color textColor(sf::Color::White);
    sf::Color borderColor(sf::Color::Yellow);
    float borderThickness = 3.0f;

    // Main menu button
    mainMenuButton.init("Main Menu", mainMenuButtonPos, buttonSize, mainMenuButtonColor, textColor, borderColor, borderThickness);
    mainMenuButton.setOnClick(std::bind(&GameOverState::onMainMenuButtonClick, this));

    // Quit button
    quitButton.init("Quit", quitButtonPos, buttonSize, quitButtonColor, textColor, borderColor, borderThickness);
    quitButton.setOnClick(std::bind(&GameOverState::onQuitButtonClick, this));
}

void GameOverState::onMainMenuButtonClick()
{
    // Transition to the MainMenuState
    gameLoop->queueStateChange(new MainMenuState(gameLoop));
}

void GameOverState::onQuitButtonClick()
{
    // Quit the game
    gameLoop->getWindow().close();
}