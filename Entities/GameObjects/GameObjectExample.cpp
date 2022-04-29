#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by true on 2022-04-27.
//

#include "GameObjectExample.h"
#include "gtest/gtest.h"

TEST(GameObjectTest, checkMoveUnitialized){

	GameObjectExample example;
	EXPECT_EQ(example.move({15,15}).getType(), SdlErrorCodeExample::Type::NOT_INITIALIZED_OBJECT);
	ASSERT_EQ(example.move({15,15}).getType(), SdlErrorCodeExample::Type::NOT_INITIALIZED_OBJECT);
}

SdlErrorCodeExample GameObjectExample::move(Position deltaPos) {
	Position curPos = pos_;
	if (!curPos.isValid() || type_ == Type::Undefined)
		return {SdlErrorCodeExample::Type::NOT_INITIALIZED_OBJECT};
	prevPos_ = curPos;
	curPos += deltaPos;
	//TODO if not wall
	return {SdlErrorCodeExample::Type::NO_ERRORS};
}

GameObjectExample::~GameObjectExample() = default;

GameObjectExample::GameObjectExample():
	pos_({-1,-1}),
	type_(GameObjectExample::Type::Undefined)
	{}

void GameObjectExample::setType(GameObjectExample::Type type) {
	type_ = type;
}

GameObjectExample::GameObjectExample(int x, int y, GameObjectExample::Type typo) {
	pos_.x_ = x;
	pos_.y_ = y;
	type_ = typo;
}

GameObjectExample::Type GameObjectExample::getType() const {
	return type_;
}
