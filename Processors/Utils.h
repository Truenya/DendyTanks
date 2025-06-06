//
// Created by true on 1/28/23.
//

#ifndef UTILS_H
#define UTILS_H
#include "uuid/uuid.h"
#include <string>

namespace uuid
{
	inline std::string generate_uuid_v4 ()
	{
		uuid_t uuid_1;
		uuid_generate (uuid_1);
		return std::string (reinterpret_cast<char *> (uuid_1));
	}
}// namespace uuid
#endif//UTILS_H
