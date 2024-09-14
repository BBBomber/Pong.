#include "../../Include/GameStates/GameplayState.h"
#include "../../Include/AudioManager.h"
#include "../../Include/AssetPaths.h"

GameplayState::GameplayState()
    : leftPaddle(sf::Vector2f(0, 250), sf::Keyboard::W, sf::Keyboard::S),   // Initialize left paddle (W and S)
    rightPaddle(sf::Vector2f(790, 250), sf::Keyboard::Up, sf::Keyboard::Down),  // Initialize right paddle (Up and Down)
    ball(10.f, 400.f), leftScore(0), rightScore(0)  //init ball and score
{
}


GameplayState::~GameplayState()
{
    // Destructor logic if necessary
}

void GameplayState::initialize()
{
    ball.initialize(); //init ball's pos and starting velocity

    // Load the font (make sure to have the font file available)
    if (!font.loadFromFile(AssetPaths::mainFont))  // Use a valid font path
    {
        // Handle font loading error
    }

    // Initialize the score text objects
    leftScoreText.setFont(font);
    leftScoreText.setCharacterSize(30);
    leftScoreText.setFillColor(sf::Color::White);
    leftScoreText.setPosition(200, 20);

    rightScoreText.setFont(font);
    rightScoreText.setCharacterSize(30);
    rightScoreText.setFillColor(sf::Color::White);
    rightScoreText.setPosition(600, 20);

    updateScoreText();  // Set initial score text
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

    ball.update(deltaTime, window);

    ball.handleCollision(leftPaddle);
    ball.handleCollision(rightPaddle);

    // Check if a goal has been scored
    checkGoal();
}

void GameplayState::render(sf::RenderWindow& window)
{
    // Render both paddles
    leftPaddle.render(window);
    rightPaddle.render(window);

    ball.render(window);

    // Render the scores
    window.draw(leftScoreText);
    window.draw(rightScoreText);
}

// Check if the ball has gone past the left or right side (goal)
void GameplayState::checkGoal()
{
    // Get the ball's position
    sf::Vector2f ballPos = ball.getBounds().getPosition();

    // Check if the ball went past the left side (right player scores)
    if (ballPos.x < 0)
    {
        rightScore++;  // Right player scores
        AudioManager::getInstance().playSoundEffect(SoundEffects::Score);
        updateScoreText();
        reset();       // Reset the ball 
    }
    // Check if the ball went past the right side (left player scores)
    else if (ballPos.x > 800)  // Assuming the window width is 800
    {
        leftScore++;   // Left player scores
        AudioManager::getInstance().playSoundEffect(SoundEffects::Score);
        updateScoreText();
        reset();       // Reset the ball 
    }
}

// Reset the ball and paddles after a goal
void GameplayState::reset()
{
    ball.reset();  // Reset the ball to the center

}

// Update the score text objects with the latest scores
void GameplayState::updateScoreText()
{
    leftScoreText.setString(std::to_string(leftScore));
    rightScoreText.setString(std::to_string(rightScore));
}