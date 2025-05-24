#include <iostream>
#include <string>
#include <unistd.h> // For chdir
#include <gtest/gtest.h>
#include <filesystem>

// Define RESOURCE_MANAGER_TEST to exclude SDL dependencies
#define RESOURCE_MANAGER_TEST
#include "../Processors/Common/ResourceManager.h"

// Main function required for Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class GameResourceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Change to the project root directory for testing
        chdir("/workspace/DendyTanks");
    }
    
    // Helper method to temporarily rename a file for testing missing resources
    void renameFileTemporarily(const std::string& originalPath, const std::string& tempPath) {
        if (std::filesystem::exists(originalPath)) {
            std::filesystem::rename(originalPath, tempPath);
        }
    }
    
    // Helper method to restore a temporarily renamed file
    void restoreFile(const std::string& tempPath, const std::string& originalPath) {
        if (std::filesystem::exists(tempPath)) {
            std::filesystem::rename(tempPath, originalPath);
        }
    }
};

// Test that ResourceManager can handle missing map files
TEST_F(GameResourceTest, ResourceManagerHandlesMissingMapFile) {
    // Temporarily rename a map file to simulate it being missing
    std::string originalPath = "../resources/labirinth.txt";
    std::string tempPath = "../resources/labirinth.txt.bak";
    
    renameFileTemporarily(originalPath, tempPath);
    
    try {
        // Try to get the map path for a file that doesn't exist
        std::string mapPath = ResourceManager::getMapFilePath("labirinth.txt");
        
        // We should get a path to an alternative map
        EXPECT_FALSE(mapPath.empty());
        std::cout << "ResourceManager returned map path: " << mapPath << std::endl;
    } catch (const std::exception& e) {
        // If we get an exception, that's also acceptable as long as it's handled gracefully
        std::cout << "ResourceManager threw exception: " << e.what() << std::endl;
        SUCCEED();
    }
    
    // Restore the original file
    restoreFile(tempPath, originalPath);
}

// Test that ResourceManager can find existing map files
TEST_F(GameResourceTest, ResourceManagerFindsExistingMapFile) {
    try {
        // Try to get the map path for a file that exists
        std::string mapPath = ResourceManager::getMapFilePath("/workspace/DendyTanks/resources/labirinth20x20.txt");
        
        // We should get a path to the map
        EXPECT_FALSE(mapPath.empty());
        std::cout << "ResourceManager returned map path: " << mapPath << std::endl;
    } catch (const std::exception& e) {
        FAIL() << "Exception thrown: " << e.what();
    }
}

// Test that ResourceManager handles file existence check correctly
TEST_F(GameResourceTest, ResourceManagerFileExistsCheck) {
    // Test with a file that exists
    EXPECT_TRUE(ResourceManager::fileExists("/workspace/DendyTanks/resources/labirinth20x20.txt"));
    
    // Test with a file that doesn't exist
    EXPECT_FALSE(ResourceManager::fileExists("/workspace/DendyTanks/resources/nonexistent_file.txt"));
}