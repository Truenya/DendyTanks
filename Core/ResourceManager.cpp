#include "ResourceManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <SDL2/SDL_image.h>

ResourceManager::ResourceManager() : renderer_(nullptr) {
}

bool ResourceManager::initialize(SDL_Renderer* renderer) {
    if (!renderer) {
        std::cerr << "ResourceManager::initialize: Invalid renderer" << std::endl;
        return false;
    }
    
    renderer_ = renderer;
    return true;
}

void ResourceManager::cleanup() {
    // Free all textures
    for (auto& [id, texture] : textures_) {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }
    textures_.clear();
    
    // Clear map cache
    mapCache_.clear();
    
    // Reset renderer reference
    renderer_ = nullptr;
}

bool ResourceManager::loadTexture(const std::string& id, const std::string& filePath) {
    // Check if texture already loaded
    if (textures_.find(id) != textures_.end()) {
        return true; // Already loaded
    }
    
    // Check if renderer is valid
    if (!renderer_) {
        std::cerr << "ResourceManager::loadTexture: Renderer not initialized" << std::endl;
        return false;
    }
    
    // Get full path from config manager
    std::string fullPath = ConfigManager::getInstance().getResourcePath(filePath);
    
    // Load texture from file
    SDL_Surface* surface = IMG_Load(fullPath.c_str());
    if (!surface) {
        std::cerr << "ResourceManager::loadTexture: Failed to load image: " 
                  << fullPath << " - " << IMG_GetError() << std::endl;
        return false;
    }
    
    // Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
    
    if (!texture) {
        std::cerr << "ResourceManager::loadTexture: Failed to create texture: " 
                  << SDL_GetError() << std::endl;
        return false;
    }
    
    // Store texture
    textures_[id] = texture;
    return true;
}

SDL_Texture* ResourceManager::getTexture(const std::string& id) const {
    auto it = textures_.find(id);
    if (it != textures_.end()) {
        return it->second;
    }
    return nullptr;
}

std::string ResourceManager::loadMapData(const std::string& filePath) {
    // Check if map is already cached
    auto it = mapCache_.find(filePath);
    if (it != mapCache_.end()) {
        return it->second;
    }
    
    // Get full path from config manager
    std::string fullPath = ConfigManager::getInstance().getResourcePath(filePath);
    
    // Load map data from file
    std::ifstream file(fullPath);
    if (!file.is_open()) {
        std::cerr << "ResourceManager::loadMapData: Failed to open map file: " << fullPath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string mapData = buffer.str();
    
    // Cache the map data
    mapCache_[filePath] = mapData;
    
    return mapData;
}