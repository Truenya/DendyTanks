//
// Created by true on 1/28/23.
//

#ifndef UTILS_H
#define UTILS_H
#include <random>
#include <sstream>
#include "uuid/uuid.h"

namespace uuid {
	std::string generate_uuid_v4() {
		uuid_t uuid_1;
		uuid_generate (uuid_1);
		return std::string (reinterpret_cast<char*>(uuid_1));
	}
}
#endif//UTILS_H
