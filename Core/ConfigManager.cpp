#include "ConfigManager.h"
#include <sstream>
#include <fstream>

ConfigManager::ConfigManager() {
    // Set default resource path
    resourceBasePath_ = "../resources/";
    
    // Set default configuration values
    configValues_["window_width"] = "800";
    configValues_["window_height"] = "600";
    configValues_["fps_limit"] = "60";
    configValues_["debug_mode"] = "false";
    configValues_["map_file"] = "labirinth.txt";
}

bool ConfigManager::loadFromFile(const std::string& configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << configPath << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Parse key=value pairs
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            // Store in config map
            configValues_[key] = value;
            
            // Update resource path if specified
            if (key == "resource_path") {
                resourceBasePath_ = value;
                // Ensure path ends with a separator
                if (!resourceBasePath_.empty() && resourceBasePath_.back() != '/' && resourceBasePath_.back() != '\\') {
                    resourceBasePath_ += '/';
                }
            }
        }
    }
    
    return true;
}

bool ConfigManager::saveToFile(const std::string& configPath) {
    std::ofstream file(configPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file for writing: " << configPath << std::endl;
        return false;
    }
    
    // Write header
    file << "# DendyTanks Configuration File" << std::endl;
    file << "# Generated automatically" << std::endl << std::endl;
    
    // Write resource path
    file << "resource_path=" << resourceBasePath_ << std::endl << std::endl;
    
    // Write all other config values
    for (const auto& [key, value] : configValues_) {
        if (key != "resource_path") {
            file << key << "=" << value << std::endl;
        }
    }
    
    return true;
}

std::string ConfigManager::getString(const std::string& key, const std::string& defaultValue) const {
    auto it = configValues_.find(key);
    if (it != configValues_.end()) {
        return it->second;
    }
    return defaultValue;
}

int ConfigManager::getInt(const std::string& key, int defaultValue) const {
    auto it = configValues_.find(key);
    if (it != configValues_.end()) {
        try {
            return std::stoi(it->second);
        } catch (const std::exception& e) {
            std::cerr << "Error converting config value to int: " << e.what() << std::endl;
        }
    }
    return defaultValue;
}

float ConfigManager::getFloat(const std::string& key, float defaultValue) const {
    auto it = configValues_.find(key);
    if (it != configValues_.end()) {
        try {
            return std::stof(it->second);
        } catch (const std::exception& e) {
            std::cerr << "Error converting config value to float: " << e.what() << std::endl;
        }
    }
    return defaultValue;
}

bool ConfigManager::getBool(const std::string& key, bool defaultValue) const {
    auto it = configValues_.find(key);
    if (it != configValues_.end()) {
        std::string value = it->second;
        // Convert to lowercase for case-insensitive comparison
        std::transform(value.begin(), value.end(), value.begin(), 
                      [](unsigned char c){ return std::tolower(c); });
        
        return value == "true" || value == "yes" || value == "1";
    }
    return defaultValue;
}

void ConfigManager::setString(const std::string& key, const std::string& value) {
    configValues_[key] = value;
}

void ConfigManager::setInt(const std::string& key, int value) {
    configValues_[key] = std::to_string(value);
}

void ConfigManager::setFloat(const std::string& key, float value) {
    configValues_[key] = std::to_string(value);
}

void ConfigManager::setBool(const std::string& key, bool value) {
    configValues_[key] = value ? "true" : "false";
}

std::string ConfigManager::getResourcePath(const std::string& resourceName) const {
    return resourceBasePath_ + resourceName;
}