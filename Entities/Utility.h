//
// Created by true on 2022-05-11.
// Updated with tests on 2025-05-23
//

#ifndef SSDL_UTILITY_H
#define SSDL_UTILITY_H

#include <vector>
#include <algorithm>

/**
 * A managed vector class that provides a simplified interface for vector operations.
 * This class maintains a count of active elements and provides methods for adding,
 * removing, and accessing elements.
 * 
 * The class uses a swap-and-pop strategy for element removal, which is efficient
 * but does not preserve element order.
 */
template <class T>
class ManagedVector {
private:
    std::vector<T> data_;  // The underlying data storage
    size_t count_ = 0;     // Number of active elements

public:
    /**
     * Default constructor
     */
    ManagedVector() = default;
    
    /**
     * Add an element to the vector
     * @param value The value to add
     */
    void add(const T& value) {
        // Ensure we have enough space
        if (data_.size() <= count_) {
            // Growth strategy: double size or use 2 for initial size
            size_t newSize = (count_ == 0) ? 2 : count_ * 2;
            data_.resize(newSize);
        }
        
        // Add the element and increment count
        data_[count_++] = value;
    }
    
    /**
     * Initialize the vector with a specific capacity
     * @param size The initial capacity
     */
    void init(size_t size) {
        data_.resize(size);
        // Note: This only sets the capacity, not the count
    }
    
    /**
     * Remove an element at a specific position using the swap-and-pop strategy.
     * This method does not preserve element order but is efficient.
     * 
     * @param position The position of the element to remove
     */
    void remove(const size_t position) {
        // Check if the position is valid
        if (count_ == 0 || position >= count_) {
            return;
        }
        
        // Move the last element to the position of the removed element
        // This is only needed if we're not removing the last element
        if (position < count_ - 1) {
            data_[position] = data_[count_ - 1];
        }
        
        // Decrement count
        --count_;
    }
    
    /**
     * Remove an element by value
     * @param value The value to remove
     * @return true if the element was found and removed, false otherwise
     */
    bool remove(const T& value) {
        // Find the element
        for (size_t i = 0; i < count_; ++i) {
            if (data_[i] == value) {
                // Remove the element at position i
                size_t pos = i;
                remove(pos);
                return true;
            }
        }
        
        // Element not found
        return false;
    }
    
    /**
     * Get the number of elements in the vector
     * @return The number of elements
     */
    [[nodiscard]] size_t count() const {
        return count_;
    }
    
    /**
     * Access an element at a specific position
     * @param position The position of the element
     * @return A reference to the element
     */
    T& operator[](size_t position) {
        return data_[position];
    }
    
    /**
     * Convert to a standard vector containing only the active elements
     * @return A standard vector with the active elements
     */
    std::vector<T> apply() const {
        std::vector<T> result;
        result.reserve(count_);
        
        for (size_t i = 0; i < count_; ++i) {
            result.push_back(data_[i]);
        }
        
        return result;
    }
};

#endif //SSDL_UTILITY_H
