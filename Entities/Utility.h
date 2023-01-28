//
// Created by true on 2022-05-11.
//

#ifndef SSDL_UTILITY_H
#define SSDL_UTILITY_H

#include <vector>
using namespace std;

template <class T>

class ManagedVector{
	vector<T> c_ {};
	size_t count_ = 0;

public:
	void add(const T t) {
		if (c_.size() < (count_ + 1))
			c_.resize(count_ * 2 + 2); // 99 -> 198 -> 200
		c_[count_] = t;
		count_++;
	}
	void init(size_t count){
		c_.resize(count);
	}
	void remove(size_t pos){
		if (count_ != 0)
		{
			std::swap(c_[pos], c_[count_ - 1]);
			count_--;
		}
	}
	bool remove(T t){
		size_t pos = count_ + 1;
		for (int i = count_; i >= 0 ; --i)
		{
			if (t == c_[i])
			{
				pos = i;
				break;
			}
		}
		if (pos != count_ + 1)
		{
		    remove(pos);
			return true;
		}
		return false;
	}
	 [[nodiscard]] size_t count() const { return count_;}
	T& operator[](size_t pos){return c_[pos];}
	std::vector<T> apply() {}
};
#endif //SSDL_UTILITY_H
