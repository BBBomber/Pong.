#include "../../Include/GameStates/MainMenuState.h"
#include "../../Include/GameStates/GameplayState.h"
#include "../../Include/GameStates/HostLobbyState.h"
#include "../../Include/AssetPaths.h"
#include <iostream>

MainMenuState::MainMenuState(GameplayLoop* loop)
    : gameLoop(loop)
{
    
}

void MainMenuState::initialize()
{

    initBackground();
    initTitle();
    initButtons();

}

void MainMenuState::handleInput(sf::RenderWindow& window, float deltaTime)
{
}

void MainMenuState::handleEventInput(const sf::Event& event, sf::RenderWindow& window)
{
 
    // Handle button events
    playButton.handleEvent(event, window);
    quitButton.handleEvent(event, window);
    HostGame.handleEvent(event, window);
    
}

void MainMenuState::update(sf::RenderWindow& window, float deltaTime)
{
    // Additional logic can be added here
}

void MainMenuState::render(sf::RenderWindow& window)
{
    // Draw background, title, and buttons
    window.clear();
    window.draw(backgroundSprite);
    window.draw(titleText);
    playButton.render(window);
    quitButton.render(window);
    HostGame.render(window);

}

void MainMenuState::onPlayButtonClick()
{
    // Queue the transition to GameplayState
    gameLoop->queueStateChange(new GameplayState(gameLoop));
}

void MainMenuState::onQuitButtonClick()
{

    gameLoop->getWindow().close();  // Close the game
}

void MainMenuState::onHostButtonClick()
{
    gameLoop->queueStateChange(new HostLobbyState(gameLoop));
    std::cout << "clicked" << std::endl;
}

void MainMenuState::initButtons()
{
    // Local variables for button parameters
    sf::Vector2f playButtonPosition(300.0f, 300.0f);
    sf::Vector2f quitButtonPosition(300.0f, 400.0f);
    sf::Vector2f hostButtonPosition(300.0f, 500.0f);
    sf::Vector2f buttonSize(200.0f, 50.0f);
    sf::Color playButtonColor(sf::Color::Green);   
    sf::Color quitButtonColor(sf::Color::Red);
    sf::Color hostButtonColor(sf::Color::Blue);
    sf::Color textColor(sf::Color::White);    
    sf::Color borderColor(sf::Color::Yellow);        
    float borderThickness = 3.0f;

    // Initialize play button
    playButton.init("Play!", playButtonPosition, buttonSize, playButtonColor, textColor, borderColor, borderThickness);
    playButton.setOnClick(std::bind(&MainMenuState::onPlayButtonClick, this));

    // Initialize quit button
    quitButton.init("Quit Game", quitButtonPosition, buttonSize, quitButtonColor, textColor, borderColor, borderThickness);
    quitButton.setOnClick(std::bind(&MainMenuState::onQuitButtonClick, this));

    HostGame.init("Host Game", hostButtonPosition, buttonSize, hostButtonColor, textColor, borderColor, borderThickness);
    HostGame.setOnClick(std::bind(&MainMenuState::onHostButtonClick, this));

}

void MainMenuState::initBackground()
{
    // Load background texture
    if (!backgroundTexture.loadFromFile(AssetPaths::bgImage))
    {
        std::cerr << "Failed to load background image!" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(800) / backgroundTexture.getSize().x,
        static_cast<float>(600) / backgroundTexture.getSize().y
    );
}

void MainMenuState::initTitle()
{
    // Load the font
    if (!font.loadFromFile(AssetPaths::mainFont))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    // Set up title text
    titleText.setFont(font);
    titleText.setString("PONG");
    titleText.setCharacterSize(72);  // Big size
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);  // Outline color
    titleText.setOutlineThickness(5.0f);          // Outline thickness
    titleText.setPosition(
        (800.0f - titleText.getLocalBounds().width) / 2.0f, // Center horizontally
        50.0f // Top padding
    );
}
