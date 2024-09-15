#pragma once

#include <string>

class AssetPaths {
public:

    // Image paths
    static const std::string bgImage;

    // Sound paths
    static const std::string bgMusic;
    static const std::string clickEffect;
    static const std::string bounceEffect;
    static const std::string scoreEffect;

    // Font paths
    static const std::string mainFont;

    // Additional asset paths can be added here

private:

    // Prevent instantiation of this static class (private constructor)
    // note that a method of this class can still instantiate this class so dont make that mistake 
    AssetPaths() {}
};
