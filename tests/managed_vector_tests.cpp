#include <gtest/gtest.h>
#include "../Entities/Utility.h"

// Test fixture for ManagedVector
class ManagedVectorTest : public ::testing::Test {
protected:
    ManagedVector<int> intVector;
    ManagedVector<std::string> stringVector;

    void SetUp() override {
        // This will be called before each test
    }

    void TearDown() override {
        // This will be called after each test
    }
};

// Test adding elements to the vector
TEST_F(ManagedVectorTest, AddElements) {
    // Add elements to the int vector
    intVector.add(1);
    intVector.add(2);
    intVector.add(3);
    
    // Check count
    EXPECT_EQ(intVector.count(), 3);
    
    // Check values
    EXPECT_EQ(intVector[0], 1);
    EXPECT_EQ(intVector[1], 2);
    EXPECT_EQ(intVector[2], 3);
    
    // Add elements to the string vector
    stringVector.add("one");
    stringVector.add("two");
    
    // Check count
    EXPECT_EQ(stringVector.count(), 2);
    
    // Check values
    EXPECT_EQ(stringVector[0], "one");
    EXPECT_EQ(stringVector[1], "two");
}

// Test initializing the vector with a specific size
TEST_F(ManagedVectorTest, InitializeVector) {
    // Initialize vector with size 5
    intVector.init(5);
    
    // Count should still be 0 as we haven't added elements
    EXPECT_EQ(intVector.count(), 0);
    
    // Add elements
    intVector.add(10);
    intVector.add(20);
    
    // Check count
    EXPECT_EQ(intVector.count(), 2);
    
    // Check values
    EXPECT_EQ(intVector[0], 10);
    EXPECT_EQ(intVector[1], 20);
}

// Test removing elements by position
TEST_F(ManagedVectorTest, RemoveByPosition) {
    // Create a fresh vector for this test
    ManagedVector<int> testVector;
    
    // Add elements
    testVector.add(100);
    testVector.add(200);
    testVector.add(300);
    testVector.add(400);
    
    // Initial count
    EXPECT_EQ(testVector.count(), 4);
    
    // Remove element at position 1 (200)
    size_t pos1 = 1;
    testVector.remove(pos1);
    
    // Count should decrease
    EXPECT_EQ(testVector.count(), 3);
    
    // Check values - the last element should have been swapped with the removed one
    EXPECT_EQ(testVector[0], 100);
    EXPECT_EQ(testVector[1], 400); // 400 was moved to position 1
    EXPECT_EQ(testVector[2], 300);
    
    // Create a new vector for the second test
    ManagedVector<int> secondVector;
    secondVector.add(100);
    secondVector.add(200);
    secondVector.add(300);
    
    // Remove element at position 0 (100)
    size_t pos2 = 0;
    secondVector.remove(pos2);
    
    // Count should decrease
    EXPECT_EQ(secondVector.count(), 2);
    
    // Check values - the last element should have been swapped with the removed one
    EXPECT_EQ(secondVector[0], 300); // 300 was moved to position 0
    EXPECT_EQ(secondVector[1], 200);
}

// Test removing elements by value
TEST_F(ManagedVectorTest, RemoveByValue) {
    // Add elements
    intVector.add(10);
    intVector.add(20);
    intVector.add(30);
    intVector.add(40);
    
    // Initial count
    EXPECT_EQ(intVector.count(), 4);
    
    // Remove element with value 20
    bool removed = intVector.remove(20);
    
    // Check if removal was successful
    EXPECT_TRUE(removed);
    
    // Count should decrease
    EXPECT_EQ(intVector.count(), 3);
    
    // Try to remove a non-existent element
    removed = intVector.remove(100);
    
    // Check if removal failed
    EXPECT_FALSE(removed);
    
    // Count should remain the same
    EXPECT_EQ(intVector.count(), 3);
}

// Test edge cases
TEST_F(ManagedVectorTest, EdgeCases) {
    // Test removing from empty vector by position
    intVector.remove(0);
    EXPECT_EQ(intVector.count(), 0);
    
    // Test removing from empty vector by value
    bool removed = intVector.remove(10);
    EXPECT_FALSE(removed);
    
    // Add a large number of elements to test resizing
    for (int i = 0; i < 50; i++) {
        intVector.add(i);
    }
    
    // Check count
    EXPECT_EQ(intVector.count(), 50);
    
    // Check some values
    EXPECT_EQ(intVector[0], 0);
    EXPECT_EQ(intVector[25], 25);
    EXPECT_EQ(intVector[49], 49);
}

// Test the apply method
TEST_F(ManagedVectorTest, ApplyMethod) {
    // Add elements
    intVector.add(5);
    intVector.add(10);
    intVector.add(15);
    
    // Get a standard vector using apply()
    std::vector<int> stdVector = intVector.apply();
    
    // Check size
    EXPECT_EQ(stdVector.size(), 3);
    
    // Check values
    EXPECT_EQ(stdVector[0], 5);
    EXPECT_EQ(stdVector[1], 10);
    EXPECT_EQ(stdVector[2], 15);
    
    // Test with empty vector
    ManagedVector<int> emptyVector;
    std::vector<int> emptyStdVector = emptyVector.apply();
    
    // Check size
    EXPECT_EQ(emptyStdVector.size(), 0);
}

// Main function
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}