//
// Created by true on 2022-04-28.
//

#include "MyGameWorld.h"

MyGameWorld::MyGameWorld(unsigned int x_dim, unsigned int y_dim)
{
	if(x_dim > 10000 || y_dim > 10000)
		throw std::logic_error ("trying to construct too big world");
	field_.resize(x_dim);
	int x = 0;
	for (auto &row: field_) {
		row.resize(y_dim);
		int y = 0;
		for (auto &point: row) {
			point.type_ = BaseGameObject::Type::UNDEFINED;
			++y;
		}
		++x;
	}
}

MyGameWorld::~MyGameWorld() = default;

Position MyGameWorld::size() {
	return {static_cast<int>(field_.size()),static_cast<int>(field_.front().size()),0,Position::Direction::UNDEFINED};
}

BaseGameObject &MyGameWorld::at(Position pos) {
 	return field_[pos.x_][pos.y_];
}
