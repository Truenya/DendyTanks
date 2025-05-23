#include <gtest/gtest.h>
#include "../Entities/Utility.h"

// Test fixture for ManagedVector
class SimpleVectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // This will be called before each test
    }

    void TearDown() override {
        // This will be called after each test
    }
};

// Test adding elements
TEST_F(SimpleVectorTest, AddElements) {
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
TEST_F(SimpleVectorTest, RemoveByPosition) {
    ManagedVector<int> vec;
    
    // Add elements
    vec.add(100);
    vec.add(200);
    vec.add(300);
    
    // Remove element at position 1 (200)
    size_t pos = 1;
    vec.remove(pos);
    
    // Count should decrease
    EXPECT_EQ(vec.count(), 2);
    
    // Check values - the last element should have been swapped with the removed one
    EXPECT_EQ(vec[0], 100);
    EXPECT_EQ(vec[1], 300);
}

// Test removing elements by value
TEST_F(SimpleVectorTest, RemoveByValue) {
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

// Test edge cases
TEST_F(SimpleVectorTest, EdgeCases) {
    ManagedVector<int> vec;
    
    // Test removing from empty vector
    vec.remove(0);
    EXPECT_EQ(vec.count(), 0);
    
    // Test removing from empty vector by value
    bool removed = vec.remove(10);
    EXPECT_FALSE(removed);
    
    // Add a large number of elements to test resizing
    for (int i = 0; i < 10; i++) {
        vec.add(i);
    }
    
    // Check count
    EXPECT_EQ(vec.count(), 10);
    
    // Check some values
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[5], 5);
    EXPECT_EQ(vec[9], 9);
}

// Test the apply method
TEST_F(SimpleVectorTest, ApplyMethod) {
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

// Main function
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}