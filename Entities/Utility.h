//
// Created by true on 2022-05-11.
//

#ifndef SSDL_UTILITY_H
#define SSDL_UTILITY_H

#include <vector>

template <class T>
class ManagedVector{
	std::vector<T> c_ {};
	size_t count_ = 0;

public:
	void add(const T t) {
		if (c_.size() < (count_ + 1))
			c_.resize(count_ + 1);
		c_[count_] = t;
		count_++;
	}
	void init(size_t count){
		c_.resize(count);
	}
	void remove(size_t pos){
		std::swap(c_[pos],c_[count_ - 1]);
		count_--;
	}
	 [[nodiscard]] size_t count() const { return count_;}
	T& operator[](size_t pos){return c_[pos];}
};

#endif //SSDL_UTILITY_H
