//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by true on 2022-04-27.
//

#include "GameObject.h"
//#include "gtest/gtest.h"
//
//TEST(GameObjectTest, checkMoveUnitialized) {
//
//	GameObject example;
//	EXPECT_EQ(example.move({15, 15,01}).getType(), SdlErrorCodeExample::Type::NOT_INITIALIZED_OBJECT);
//	ASSERT_EQ(example.move({15, 15}).getType(), SdlErrorCodeExample::Type::NOT_INITIALIZED_OBJECT);
//}

//SdlErrorCodeExample GameObject::move(Position delta_pos) {
//	Position cur_pos = positions_.curPos_;
//
//	// Проверка на выход за границы карты и инициализированность объекта
//	if (!cur_pos || type_ == Type::EQUAL)
//		return {SdlErrorCodeExample::Type::NOT_INITIALIZED_OBJECT};
//	auto new_pos = cur_pos + delta_pos;
////	if (!(new_pos && (*field_)[new_pos.x_][new_pos.y_].type_ == GameObject::Type::SPACE))
//	if (!(new_pos && (*field_)[new_pos.x_][new_pos.y_] == GameObject::Type::SPACE))
//		return {SdlErrorCodeExample::Type::CANNOT_MOVE};
//	positions_.prevPos_ = cur_pos;
////	cur_pos += delta_pos;
//	positions_.curPos_ = new_pos;
//	positions_.curPos_.direction_ = delta_pos.direction_;
//	positions_.dPos_ = delta_pos;
////	std::swap((*field_)[cur_pos.x_][cur_pos.y_], (*field_)[new_pos.x_][new_pos.y_]);
//	(*field_)[cur_pos.x_][cur_pos.y_].positions_.prevPos_ = new_pos;
//	(*field_)[cur_pos.x_][cur_pos.y_].positions_.dPos_ = delta_pos;
//	(*field_)[cur_pos.x_][cur_pos.y_].positions_.curPos_ = cur_pos;
//	return {SdlErrorCodeExample::Type::NO_ERRORS};
//}

GameObject::~GameObject() = default;

GameObject::GameObject() :
		type_(GameObject::Type::UNDEFINED),
		positions_({{-1, -1, 0, Position::Direction::BOT},
		            {-1, -1, 0, Position::Direction::BOT}})
//                 ,
//	                    {-1, -1, 0, Position::Direction::BOT},
//						{-1, -1, 0, Position::Direction::BOT}})
//		field_(nullptr)
{}

const Positions &GameObject::getPositions() const {
	return positions_;
}

GameObject::GameObject(const Position& pos, GameObject::Type typo)//,
		//std::vector<std::vector<GameObject::Type>> *field)
		:
		type_(typo),
		positions_({pos, pos})//, {0, 0, 0, Position::Direction::BOT},pos})
//		field_(field)
{}

void GameObject::step() {
	positions_.prevPos_ = positions_.curPos_;
	positions_.curPos_.stepInDirection();
}

void GameObject::rotate (Position::Direction direction)
{
	positions_.curPos_.direction_ = direction;
}
