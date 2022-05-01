//
// Created by true on 2022-04-27.
//
#include "Position.h"

void Position::move(int x,int y,int z){
	x += x;
	y += y;
	z += z;
}

Position& Position::operator+=(const Position &other) {
	x+=other.x;
	y+=other.y;
	z+=other.z;
	return *this;
}

Position& Position::operator+=(const Position &&other) {
	x+=other.x;
	y+=other.y;
	z+=other.z;
	return *this;
}

bool Position::isValid() const {
	if ((x <= -1 ) || (y <= -1) || (z <= -1))
		return false;
	return true;
}

bool Position::operator==(const Position &rhs) const {
	return x == rhs.x &&
	       y == rhs.y &&
	       z == rhs.z;
}

bool Position::operator!=(const Position &rhs) const {
	return !(rhs == *this);
}

bool Position::operator<(const Position &rhs) const {
	if (x < rhs.x)
		return true;
	if (rhs.x < x)
		return false;
	if (y < rhs.y)
		return true;
	if (rhs.y < y)
		return false;
	return z < rhs.z;
}

bool Position::operator>(const Position &rhs) const {
	return rhs < *this;
}

bool Position::operator<=(const Position &rhs) const {
	return !(rhs < *this);
}

bool Position::operator>=(const Position &rhs) const {
	return !(*this < rhs);
}


Position Position::operator+(const Position &other) const {
	Position position;
	position.x = x+other.x;
	position.y = y+other.y;
	position.z = z+other.z;
	return position;
}

Position Position::operator+(const Position &&other) const {
	Position position;
	position.x = x+other.x;
	position.y = y+other.y;
	position.z = z+other.z;
	return position;
}

Position Position::operator-(const Position &other) const {
	Position position;
	position.x = x-other.x;
	position.y = y-other.y;
	position.z = z-other.z;
	return position;
}

Position Position::operator-(const Position &&other) const {
	Position position;
	position.x = x-other.x;
	position.y = y-other.y;
	position.z = z-other.z;
	return position;
}

Position& Position::operator-=(const Position &other) {
	x-=other.x;
	y-=other.y;
	z-=other.z;
	return *this;
}

Position& Position::operator-=(const Position &&other) {
	x-=other.x;
	y-=other.y;
	z-=other.z;
	return *this;
}

Position::operator bool() const {
	return isValid();
}

Position Position::operator-() const {
	Position minus = *this;
	minus.x = -minus.x;
	minus.y = -minus.y;
	minus.z = -minus.z;
	return minus;
}
