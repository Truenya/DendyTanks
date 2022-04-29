//
// Created by true on 2022-04-28.
//

#include "MyGameWorld.h"

#include <memory>


MyGameWorld::MyGameWorld(unsigned int x_dim, unsigned int y_dim) {

	//TODO Добавить проверки, вынести инициализацию из конструктора

	for (int x = 0; x < x_dim; ++x) {
		field_.emplace_back(std::vector<ptr_GamObjExample>(y_dim));

		for (int y = 0; y < y_dim; ++y) {
			auto grbg = field_[x][y].release();
			field_[x][y] = std::make_unique<GameObjectExample>(x,y,GameObjectExample::Type::Undefined);
		}
	}
}

MyGameWorld::~MyGameWorld() {

}

GameObjectExample * MyGameWorld::at(int x, int y) const{
	return field_[x][y].get();
}

coordinates MyGameWorld::size() {
	return {field_.size(),field_.front().size()};
}
