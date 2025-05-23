#ifndef DENDY_TANKS_CONFIG_MANAGER_H
#define DENDY_TANKS_CONFIG_MANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <iostream>
#include <filesystem>

/**
 * @class ConfigManager
 * @brief Manages game configuration settings
 * 
 * This class provides a centralized way to manage game configuration settings,
 * including resource paths, game settings, and other configuration options.
 */
class ConfigManager {
public:
    /**
     * @brief Get the singleton instance of the ConfigManager
     * @return Reference to the ConfigManager instance
     */
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    /**
     * @brief Load configuration from a file
     * @param configPath Path to the configuration file
     * @return True if loading was successful, false otherwise
     */
    bool loadFromFile(const std::string& configPath);

    /**
     * @brief Save configuration to a file
     * @param configPath Path to the configuration file
     * @return True if saving was successful, false otherwise
     */
    bool saveToFile(const std::string& configPath);

    /**
     * @brief Get a string value from the configuration
     * @param key The configuration key
     * @param defaultValue Default value to return if key is not found
     * @return The configuration value or defaultValue if not found
     */
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;

    /**
     * @brief Get an integer value from the configuration
     * @param key The configuration key
     * @param defaultValue Default value to return if key is not found
     * @return The configuration value or defaultValue if not found
     */
    int getInt(const std::string& key, int defaultValue = 0) const;

    /**
     * @brief Get a float value from the configuration
     * @param key The configuration key
     * @param defaultValue Default value to return if key is not found
     * @return The configuration value or defaultValue if not found
     */
    float getFloat(const std::string& key, float defaultValue = 0.0f) const;

    /**
     * @brief Get a boolean value from the configuration
     * @param key The configuration key
     * @param defaultValue Default value to return if key is not found
     * @return The configuration value or defaultValue if not found
     */
    bool getBool(const std::string& key, bool defaultValue = false) const;

    /**
     * @brief Set a string value in the configuration
     * @param key The configuration key
     * @param value The value to set
     */
    void setString(const std::string& key, const std::string& value);

    /**
     * @brief Set an integer value in the configuration
     * @param key The configuration key
     * @param value The value to set
     */
    void setInt(const std::string& key, int value);

    /**
     * @brief Set a float value in the configuration
     * @param key The configuration key
     * @param value The value to set
     */
    void setFloat(const std::string& key, float value);

    /**
     * @brief Set a boolean value in the configuration
     * @param key The configuration key
     * @param value The value to set
     */
    void setBool(const std::string& key, bool value);

    /**
     * @brief Get the resource path for a given resource
     * @param resourceName The name of the resource
     * @return The full path to the resource
     */
    std::string getResourcePath(const std::string& resourceName) const;

private:
    // Private constructor for singleton pattern
    ConfigManager();
    
    // Prevent copying and assignment
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    // Configuration storage
    std::unordered_map<std::string, std::string> configValues_;
    
    // Base path for resources
    std::string resourceBasePath_;
};

#endif // DENDY_TANKS_CONFIG_MANAGER_H