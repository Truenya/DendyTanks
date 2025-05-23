#include <gtest/gtest.h>
#include "../Entities/Utility.h"

// Test fixture for ManagedVector
class NewManagedVectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // This will be called before each test
    }

    void TearDown() override {
        // This will be called after each test
    }
};

// Test adding elements
TEST_F(NewManagedVectorTest, AddElements) {
    ManagedVector<int> vec;
    
    // Add elements
    vec.add(10);
    vec.add(20);
    
    // Check count
    EXPECT_EQ(vec.count(), 2);
    
    // Check values
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
}

// Test removing elements by position
TEST_F(NewManagedVectorTest, RemoveByPosition) {
    ManagedVector<int> vec;
    
    // Add elements
    vec.add(100);
    vec.add(200);
    vec.add(300);
    vec.add(400);
    
    // Initial count
    EXPECT_EQ(vec.count(), 4);
    
    // Remove element at position 1 (200)
    vec.remove(1);
    
    // Count should decrease
    EXPECT_EQ(vec.count(), 3);
    
    // Check values - the last element should have been swapped with the removed one
    EXPECT_EQ(vec[0], 100);
    EXPECT_EQ(vec[1], 400); // 400 was moved to position 1
    EXPECT_EQ(vec[2], 300);
}

// Test removing elements by value
TEST_F(NewManagedVectorTest, RemoveByValue) {
    ManagedVector<int> vec;
    
    // Add elements
    vec.add(100);
    vec.add(200);
    vec.add(300);
    
    // Remove element with value 200
    bool removed = vec.remove(200);
    
    // Should return true
    EXPECT_TRUE(removed);
    
    // Count should decrease
    EXPECT_EQ(vec.count(), 2);
    
    // Check values
    EXPECT_EQ(vec[0], 100);
    EXPECT_EQ(vec[1], 300);
    
    // Try to remove a non-existent element
    removed = vec.remove(999);
    
    // Should return false
    EXPECT_FALSE(removed);
    
    // Count should remain the same
    EXPECT_EQ(vec.count(), 2);
}

// Test the apply method
TEST_F(NewManagedVectorTest, ApplyMethod) {
    ManagedVector<int> vec;
    
    // Add elements
    vec.add(100);
    vec.add(200);
    vec.add(300);
    
    // Get standard vector
    std::vector<int> stdVec = vec.apply();
    
    // Check size
    EXPECT_EQ(stdVec.size(), 3);
    
    // Check values
    EXPECT_EQ(stdVec[0], 100);
    EXPECT_EQ(stdVec[1], 200);
    EXPECT_EQ(stdVec[2], 300);
}