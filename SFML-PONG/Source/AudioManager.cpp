#include "../Include/AudioManager.h"
#include "../Include/AssetPaths.h"
#include <iostream>

// Static member definition
AudioManager AudioManager::instance;

// Singleton instance getter
AudioManager& AudioManager::getInstance()
{
    return instance;
}

// Initialize the audio manager by loading all sound effects and music
void AudioManager::initialize()
{
    if (!soundBuffers[0].loadFromFile(AssetPaths::clickEffect))
    {
        logError("Failed to load Click sound effect");
    }
    sounds[0].setBuffer(soundBuffers[0]);

    if (!soundBuffers[1].loadFromFile(AssetPaths::scoreEffect))
    {
        logError("Failed to load Score sound effect");
    }
    sounds[1].setBuffer(soundBuffers[1]);

    if (!soundBuffers[2].loadFromFile(AssetPaths::bounceEffect))
    {
        logError("Failed to load Bounce sound effect");
    }
    sounds[2].setBuffer(soundBuffers[2]);

    // Load background music
    if (!backgroundMusic.openFromFile(AssetPaths::bgMusic))
    {
        logError("Failed to open background music file");
    }
}

// Play a sound effect based on the enum
void AudioManager::playSoundEffect(SoundEffects effect)
{
    switch (effect)
    {
    case SoundEffects::Click:
        sounds[0].play();
        break;
    case SoundEffects::Score:
        sounds[1].play();
        break;
    case SoundEffects::Bounce:
        sounds[2].play();
        break;
    default:
        logError("Invalid sound effect enum");
        break;
    }
}

// Play background music (looping if specified)
void AudioManager::playBackgroundMusic(bool loop)
{
    backgroundMusic.setLoop(loop);
    backgroundMusic.play();
}

// Stop the background music
void AudioManager::stopBackgroundMusic()
{
    backgroundMusic.stop();
}

// Helper function to log error messages
void AudioManager::logError(const std::string& message)
{
    std::cerr << message << std::endl;
}