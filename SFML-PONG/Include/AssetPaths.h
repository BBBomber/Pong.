#pragma once

#include <string>

class AssetPaths {
public:

    // Image paths

    // Sound paths

    // Font paths
    static const std::string mainFont;

    // Additional asset paths can be added here

private:

    // Prevent instantiation of this static class (private constructor)
    // note that a method of this class can still instantiate this class so dont make that mistake 
    AssetPaths() {}
};
