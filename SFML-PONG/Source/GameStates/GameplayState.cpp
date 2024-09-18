#include "../../Include/GameStates/GameplayState.h"

GameplayState::GameplayState(GameplayLoop* loop)
    : gameLoop(loop)
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
    // Input handling logic (empty for now)
}

void GameplayState::handleEventInput(const sf::Event& event, sf::RenderWindow& window)
{
}

void GameplayState::update(sf::RenderWindow& window, float deltaTime)
{
    // Update gameplay logic (empty for now)
}

void GameplayState::render(sf::RenderWindow& window)
{
    // Rendering logic (empty for now)
}