#ifndef DENDY_TANKS_RESOURCE_MANAGER_H
#define DENDY_TANKS_RESOURCE_MANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <SDL2/SDL.h>
#include "ConfigManager.h"

/**
 * @class ResourceManager
 * @brief Manages game resources like textures, sounds, and maps
 * 
 * This class provides a centralized way to load and manage game resources,
 * including textures, sounds, and map data.
 */
class ResourceManager {
public:
    /**
     * @brief Get the singleton instance of the ResourceManager
     * @return Reference to the ResourceManager instance
     */
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    /**
     * @brief Initialize the resource manager
     * @param renderer SDL renderer to use for texture loading
     * @return True if initialization was successful, false otherwise
     */
    bool initialize(SDL_Renderer* renderer);

    /**
     * @brief Clean up resources
     */
    void cleanup();

    /**
     * @brief Load a texture from file
     * @param id Identifier for the texture
     * @param filePath Path to the texture file
     * @return True if loading was successful, false otherwise
     */
    bool loadTexture(const std::string& id, const std::string& filePath);

    /**
     * @brief Get a texture by ID
     * @param id Identifier for the texture
     * @return Pointer to the texture, or nullptr if not found
     */
    SDL_Texture* getTexture(const std::string& id) const;

    /**
     * @brief Load a map from file
     * @param filePath Path to the map file
     * @return The map data as a string
     */
    std::string loadMapData(const std::string& filePath);

private:
    // Private constructor for singleton pattern
    ResourceManager();
    
    // Prevent copying and assignment
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // SDL renderer reference
    SDL_Renderer* renderer_;
    
    // Texture storage
    std::unordered_map<std::string, SDL_Texture*> textures_;
    
    // Map data cache
    std::unordered_map<std::string, std::string> mapCache_;
};

#endif // DENDY_TANKS_RESOURCE_MANAGER_H