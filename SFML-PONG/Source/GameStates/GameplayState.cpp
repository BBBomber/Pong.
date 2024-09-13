#include "../../Include/GameStates/GameplayState.h"

GameplayState::GameplayState()
    : leftPaddle(sf::Vector2f(0, 250), sf::Keyboard::W, sf::Keyboard::S),   // Initialize left paddle (W and S)
    rightPaddle(sf::Vector2f(790, 250), sf::Keyboard::Up, sf::Keyboard::Down)  // Initialize right paddle (Up and Down)
{
}


GameplayState::~GameplayState()
{
    // Destructor logic if necessary
}

void GameplayState::initialize()
{
    // Initialization logic for gameplay (empty for now)
}

void GameplayState::handleInput(sf::RenderWindow& window, float deltaTime)
{
    // Delegate input handling to the paddles
    leftPaddle.handleInput(window, deltaTime);
    rightPaddle.handleInput(window, deltaTime);
}

void GameplayState::update(sf::RenderWindow& window, float deltaTime)
{
    // Update both paddles with boundary checks
    leftPaddle.update(deltaTime, window);
    rightPaddle.update(deltaTime, window);
}

void GameplayState::render(sf::RenderWindow& window)
{
    // Render both paddles
    leftPaddle.render(window);
    rightPaddle.render(window);
}