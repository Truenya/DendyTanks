//
// Created by true on 2022-04-28.
//

#include "MyGameWorld.h"

MyGameWorld::MyGameWorld(unsigned int x_dim, unsigned int y_dim) {

	//TODO Добавить проверки, вынести инициализацию из конструктора
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
//	for (int x_ = 0; x_ < x_dim; ++x_) {
////		field_.emplace_back(std::vector<ptr_GamObjExample>(y_dim));
//		field_.emplace_back(std::vector<BaseGameObject>(y_dim));
//
//		for (int y_ = 0; y_ < y_dim; ++y_) {
////			auto grbg = field_[x_][y_].release();
////			field_[x_][y_] = std::make_unique<BaseGameObject>(x_,y_,BaseGameObject::Type::UNDEFINED);
//			field_[x_][y_].stepInDirection(Position{x_+1,y_+1});
//			field_[x_][y_].setType(BaseGameObject::Type::UNDEFINED);
//		}
//	}
}

MyGameWorld::~MyGameWorld() = default;

Position MyGameWorld::size() {
	return {static_cast<int>(field_.size()),static_cast<int>(field_.front().size()),0,Position::Direction::UNDEFINED};
}

BaseGameObject &MyGameWorld::at(Position pos) {
 	return field_[pos.x_][pos.y_];
}
