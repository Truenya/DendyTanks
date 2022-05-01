//
// Created by true on 2022-04-28.
//

#include "MyGameWorld.h"

#include <memory>


MyGameWorld::MyGameWorld(unsigned int x_dim, unsigned int y_dim) {

	//TODO Добавить проверки, вынести инициализацию из конструктора
	field_.resize(x_dim);
	int x = 0;
	for (auto &row: field_) {
		row.resize(y_dim);
		int y = 0;
		for (auto &point: row) {
			point.type = BaseGameObject::Type::Undefined;
			++y;
		}
		++x;
	}
//	for (int x = 0; x < x_dim; ++x) {
////		field_.emplace_back(std::vector<ptr_GamObjExample>(y_dim));
//		field_.emplace_back(std::vector<BaseGameObject>(y_dim));
//
//		for (int y = 0; y < y_dim; ++y) {
////			auto grbg = field_[x][y].release();
////			field_[x][y] = std::make_unique<BaseGameObject>(x,y,BaseGameObject::Type::Undefined);
//			field_[x][y].move(Position{x+1,y+1});
//			field_[x][y].setType(BaseGameObject::Type::Undefined);
//		}
//	}
}

MyGameWorld::~MyGameWorld() {

}

Position MyGameWorld::size() {
	return {static_cast<int>(field_.size()),static_cast<int>(field_.front().size())};
}

BaseGameObject &MyGameWorld::at(Position pos) {
 	return field_[pos.x][pos.y];
}
