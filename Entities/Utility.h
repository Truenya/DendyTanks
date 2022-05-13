//
// Created by true on 2022-05-11.
//

#ifndef SSDL_UTILITY_H
#define SSDL_UTILITY_H

#include <vector>

template <class T>
struct ManagedVector{
	std::vector<T> c {};
	size_t count = 0;
};

#endif //SSDL_UTILITY_H
