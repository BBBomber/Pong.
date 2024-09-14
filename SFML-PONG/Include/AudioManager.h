#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include "SoundEffects.h"

class AudioManager
{
public:
    // Singleton pattern - Get the instance of AudioManager
    static AudioManager& getInstance();

    // Load sound effects and background music
    void initialize();
    void playSoundEffect(SoundEffects effect);  // Play a sound effect
    void playBackgroundMusic(bool loop = true);  // Play background music (optionally loop)
    void stopBackgroundMusic();  // Stop background music

private:
    AudioManager() {}  // Private constructor for singleton

    // Static instance of the class (singleton)
    static AudioManager instance;

    // Sound storage
    sf::SoundBuffer soundBuffers[3];  // Array for sound buffers (one per sound effect)
    sf::Sound sounds[3];              // Array for sounds (one per sound effect)
    sf::Music backgroundMusic;        // Object to manage background music

    void logError(const std::string& message);  // Log errors
};