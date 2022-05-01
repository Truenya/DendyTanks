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
	mDirection=other.mDirection;
	return *this;
}

Position& Position::operator+=(const Position &&other) {
	x+=other.x;
	y+=other.y;
	z+=other.z;
	mDirection=other.mDirection;
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
	position.mDirection=other.mDirection;
	return position;
}

Position Position::operator+(const Position &&other) const {
	Position position;
	position.x = x+other.x;
	position.y = y+other.y;
	position.z = z+other.z;
	position.mDirection=other.mDirection;
	return position;
}

Position Position::operator-(const Position &other) const {
	Position position;
	position.x = x-other.x;
	position.y = y-other.y;
	position.z = z-other.z;
	position.mDirection=other.mDirection;
	return position;
}

Position Position::operator-(const Position &&other) const {
	Position position;
	position.x = x-other.x;
	position.y = y-other.y;
	position.z = z-other.z;
	position.mDirection=other.mDirection;
	return position;
}

Position& Position::operator-=(const Position &other) {
	x-=other.x;
	y-=other.y;
	z-=other.z;
	mDirection=other.mDirection;
	return *this;
}

Position& Position::operator-=(const Position &&other) {
	x-=other.x;
	y-=other.y;
	z-=other.z;
	mDirection=other.mDirection;
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
	if (mDirection == Direction::Top){
		minus.mDirection = Direction::Bot;
	}
	if (mDirection == Direction::Bot){
		minus.mDirection = Direction::Top;
	}
	if (mDirection == Direction::Left){
		minus.mDirection = Direction::Right;
	}
	if (mDirection == Direction::Left){
		minus.mDirection = Direction::Right;
	}
	return minus;
}

void Position::stepInDirection(bool inverse) {
	// TODO сложная логика, преобразовать поправить сделать единообразной
	if (inverse)
		reverseDirection();
	switch (mDirection) {
		case Direction::Top:{
			this->operator+=(Position{0,-1,0,Direction::Top});
			break;
		}
		case Direction::Bot:{
			this->operator+=(Position{0,1,0,Direction::Bot});
			break;
		}
		case Direction::Left:{
			this->operator+=(Position{-1,0,0,Direction::Bot});
			break;
		}
		case Direction::Right:{
			this->operator+=(Position{1,0,0,Direction::Bot});
			break;
		}
	}
}

void Position::reverseDirection() {

	switch (mDirection) {
		case Direction::Top:{
			mDirection = Direction::Bot;
			break;
		}
		case Direction::Bot:{
			mDirection = Direction::Top;
			break;
		}
		case Direction::Left:{
			mDirection = Direction::Right;
			break;
		}
		case Direction::Right:{
			mDirection = Direction::Left;
			break;
		}
	}
}
