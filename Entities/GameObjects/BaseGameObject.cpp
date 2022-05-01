#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by true on 2022-04-27.
//

#include "BaseGameObject.h"
#include "gtest/gtest.h"

TEST(GameObjectTest, checkMoveUnitialized){

	BaseGameObject example;
	EXPECT_EQ(example.move({15,15}).getType(), SdlErrorCodeExample::Type::NOT_INITIALIZED_OBJECT);
	ASSERT_EQ(example.move({15,15}).getType(), SdlErrorCodeExample::Type::NOT_INITIALIZED_OBJECT);
}

SdlErrorCodeExample BaseGameObject::move(Position deltaPos) {
	Position curPos = positions_.curPos;

	// Проверка на выход за границы карты и инициализированность объекта
	if (!curPos || type == Type::Undefined)
		return {SdlErrorCodeExample::Type::NOT_INITIALIZED_OBJECT};
	auto newPos = curPos + deltaPos;
	if (!(newPos && (*field_)[newPos.x][newPos.y].type == BaseGameObject::Type::Space))
		return {SdlErrorCodeExample::Type::CANNOT_MOVE};
	positions_.prevPos = curPos;
//	curPos += deltaPos;
	positions_.curPos = newPos;
	positions_.dPos = std::move(deltaPos);
	std::swap((*field_)[curPos.x][curPos.y],(*field_)[newPos.x][newPos.y]);
	(*field_)[curPos.x][curPos.y].positions_.prevPos = newPos;
	(*field_)[curPos.x][curPos.y].positions_.dPos = deltaPos;
	(*field_)[curPos.x][curPos.y].positions_.curPos = curPos;
	return {SdlErrorCodeExample::Type::NO_ERRORS};
}

BaseGameObject::~BaseGameObject() = default;

BaseGameObject::BaseGameObject():
		positions_({{-1, -1, 0},
	            {-1, -1, 0},
	            {-1, -1, 0}}),
		type(BaseGameObject::Type::Undefined)
	{}

BaseGameObject::BaseGameObject(int x, int y, BaseGameObject::Type typo) {
	positions_.curPos.x = x;
	positions_.curPos.y = y;
	type = typo;
}

const Positions &BaseGameObject::getPositions() const {
	return positions_;
}

BaseGameObject::BaseGameObject(Position pos, BaseGameObject::Type typo,std::vector<std::vector<BaseGameObject>> *field):
	positions_({pos,pos,{0,0,0}}),
	type(typo),
	field_(field)
{}
