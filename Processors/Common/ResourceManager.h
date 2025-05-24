#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <stdexcept>

// Only include SDL headers when not in test mode
#ifndef RESOURCE_MANAGER_TEST
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

namespace fs = std::filesystem;

class ResourceManager {
public:
    static std::string getMapFilePath(const std::string& mapName = "default") {
        // First try the exact path
        if (fileExists(mapName)) {
            return mapName;
        }

        // Try with resources prefix
        std::string resourcePath = "../resources/" + mapName;
        if (fileExists(resourcePath)) {
            return resourcePath;
        }

        // Try default maps
        if (mapName == "default" || mapName == "labirinth.txt") {
            // Try labirinth20x20.txt
            resourcePath = "../resources/labirinth20x20.txt";
            if (fileExists(resourcePath)) {
                std::cout << "Using default map: " << resourcePath << std::endl;
                return resourcePath;
            }

            // Try labirinth50x50.txt
            resourcePath = "../resources/labirinth50x50.txt";
            if (fileExists(resourcePath)) {
                std::cout << "Using default map: " << resourcePath << std::endl;
                return resourcePath;
            }
        }

        // If we get here, we couldn't find a suitable map file
        throw std::runtime_error("Could not find map file: " + mapName + 
            "\nPlease make sure the map file exists in the resources directory.\n"
            "Available maps: labirinth20x20.txt, labirinth50x50.txt");
    }

    static bool fileExists(const std::string& path) {
        std::ifstream file(path);
        return file.good();
    }

    static std::string getBaseName(const std::string& path) {
        size_t pos = path.find_last_of("/\\");
        if (pos != std::string::npos) {
            return path.substr(pos + 1);
        }
        return path;
    }

#ifndef RESOURCE_MANAGER_TEST
    static SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& textureName) {
        // First try the exact path
        if (fileExists(textureName)) {
            return loadTextureFromPath(renderer, textureName);
        }

        // Try with resources prefix
        std::string resourcePath = "../resources/" + textureName;
        if (fileExists(resourcePath)) {
            return loadTextureFromPath(renderer, resourcePath);
        }

        // Try to find a matching texture in the resources directory
        std::string baseName = getBaseName(textureName);
        
        // Handle special cases for tank textures
        if (baseName == "tank_b.png") {
            resourcePath = "../resources/tanks_b_green_blue_red_512x605.png";
            if (fileExists(resourcePath)) {
                std::cout << "Using texture: " << resourcePath << " for " << textureName << std::endl;
                return loadTextureFromPath(renderer, resourcePath);
            }
        } else if (baseName == "tank_t.png") {
            resourcePath = "../resources/tanks_t_green_blue_red_512x605.png";
            if (fileExists(resourcePath)) {
                std::cout << "Using texture: " << resourcePath << " for " << textureName << std::endl;
                return loadTextureFromPath(renderer, resourcePath);
            }
        } else if (baseName == "tank_l.png") {
            resourcePath = "../resources/tanks_l_green_blue_red_512x605.png";
            if (fileExists(resourcePath)) {
                std::cout << "Using texture: " << resourcePath << " for " << textureName << std::endl;
                return loadTextureFromPath(renderer, resourcePath);
            }
        } else if (baseName == "tank_r.png") {
            resourcePath = "../resources/tanks_r_green_blue_red_512x605.png";
            if (fileExists(resourcePath)) {
                std::cout << "Using texture: " << resourcePath << " for " << textureName << std::endl;
                return loadTextureFromPath(renderer, resourcePath);
            }
        } else if (baseName == "explosion.png") {
            resourcePath = "../resources/explosion.png";
            if (fileExists(resourcePath)) {
                return loadTextureFromPath(renderer, resourcePath);
            }
        } else if (baseName == "cvetok.png") {
            resourcePath = "../resources/cvetok.png";
            if (fileExists(resourcePath)) {
                return loadTextureFromPath(renderer, resourcePath);
            }
        } else if (baseName == "fill.png") {
            resourcePath = "../resources/fill.png";
            if (fileExists(resourcePath)) {
                return loadTextureFromPath(renderer, resourcePath);
            }
        }

        // If we get here, we couldn't find a suitable texture
        throw std::runtime_error("Could not find texture: " + textureName + 
            "\nPlease make sure the texture file exists in the resources directory.");
    }

    static SDL_Texture* loadTextureFromPath(SDL_Renderer* renderer, const std::string& path) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (surface == nullptr) {
            throw std::runtime_error("Failed to load image: " + path + "\nError: " + IMG_GetError());
        }
        
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        
        if (texture == nullptr) {
            throw std::runtime_error("Failed to create texture from surface: " + path + "\nError: " + SDL_GetError());
        }
        
        return texture;
    }
#endif
};