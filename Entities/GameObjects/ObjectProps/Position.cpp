//
// Created by true on 2022-04-27.
//
#include "Position.h"

void Position::move(int x,int y,int z){
    x_ += x;
    y_ += y;
    z_ += z;
}

Position Position::operator+=(const Position &other) {
	this->x_+=other.x_;
	this->y_+=other.y_;
	this->z_+=other.z_;
	return *this;
}

Position Position::operator+=(const Position &&other) {
	this->x_+=other.x_;
	this->y_+=other.y_;
	this->z_+=other.z_;
	return *this;
}

bool Position::isValid() {
	if ((x_ <= -1 ) || (y_ <= -1) || (z_ <= -1))
		return false;
	return true;
}
