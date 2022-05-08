#pragma clang diagnostic push
//#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by true on 2022-04-27.
//

#include "BaseGameObject.h"
//#include "gtest/gtest.h"
//
//TEST(GameObjectTest, checkMoveUnitialized) {
//
//	BaseGameObject example;
//	EXPECT_EQ(example.move({15, 15,01}).getType(), SdlErrorCodeExample::Type::NOT_INITIALIZED_OBJECT);
//	ASSERT_EQ(example.move({15, 15}).getType(), SdlErrorCodeExample::Type::NOT_INITIALIZED_OBJECT);
//}

SdlErrorCodeExample BaseGameObject::move(Position delta_pos) {
	Position cur_pos = positions_.curPos_;

	// Проверка на выход за границы карты и инициализированность объекта
	if (!cur_pos || type_ == Type::UNDEFINED)
		return {SdlErrorCodeExample::Type::NOT_INITIALIZED_OBJECT};
	auto new_pos = cur_pos + delta_pos;
	if (!(new_pos && (*field_)[new_pos.x_][new_pos.y_].type_ == BaseGameObject::Type::SPACE))
		return {SdlErrorCodeExample::Type::CANNOT_MOVE};
	positions_.prevPos_ = cur_pos;
//	cur_pos += delta_pos;
	positions_.curPos_ = new_pos;
	positions_.curPos_.mDirection_ = delta_pos.mDirection_;
	positions_.dPos_ = delta_pos;
	std::swap((*field_)[cur_pos.x_][cur_pos.y_], (*field_)[new_pos.x_][new_pos.y_]);
	(*field_)[cur_pos.x_][cur_pos.y_].positions_.prevPos_ = new_pos;
	(*field_)[cur_pos.x_][cur_pos.y_].positions_.dPos_ = delta_pos;
	(*field_)[cur_pos.x_][cur_pos.y_].positions_.curPos_ = cur_pos;
	return {SdlErrorCodeExample::Type::NO_ERRORS};
}

BaseGameObject::~BaseGameObject() = default;

BaseGameObject::BaseGameObject() :
		type_(BaseGameObject::Type::UNDEFINED),
		positions_({{-1, -1, 0, Position::Direction::BOT},
		            {-1, -1, 0, Position::Direction::BOT},
	                    {-1, -1, 0, Position::Direction::BOT},
						{-1, -1, 0, Position::Direction::BOT}}),
		field_(nullptr){}

const Positions &BaseGameObject::getPositions() const {
	return positions_;
}

BaseGameObject::BaseGameObject(Position pos, BaseGameObject::Type typo, std::vector<std::vector<BaseGameObject>> *field)
		:
		type_(typo),
		positions_({pos, pos, {0, 0, 0, Position::Direction::BOT},pos}),
		field_(field) {}

BaseGameObject::StepReturn BaseGameObject::step() {
//	TODO using step for player, not move(deltaPosition)
//	if(type_ == BaseGameObject::Type::PLAYER)
//	{
//		auto next_pos = positions_.curPos_;
//		next_pos.stepInDirection();
//		auto next_pos_type = (*field_)[next_pos.x_][next_pos.y_].type_;
//		if (next_pos_type == BaseGameObject::Type::WALL     ||
//			next_pos_type == BaseGameObject::Type::PLAYER   ||
//			next_pos_type == BaseGameObject::Type::UNDEFINED) {
//			return StepReturn::MEET_WALL;
//		}
//		positions_.prevPos_ = positions_.curPos_;
//		positions_.curPos_ = next_pos;
//		positions_.dPos_ = next_pos-positions_.prevPos_;
//
//		if (next_pos_type == BaseGameObject::Type::SPACE) {
//			(*field_)[next_pos.x_][next_pos.y_].type_ = BaseGameObject::Type::PLAYER;
//			(*field_)[positions_.prevPos_.x_][positions_.prevPos_.y_].type_ = BaseGameObject::Type::SPACE;
//			return StepReturn::SUCCESS;
//		}
//		// TODO экран поражения
//		return StepReturn::MEET_PROJECTILE;
//	}
	bool proj = type_ == BaseGameObject::Type::PROJECTILE;
	bool posvalid = positions_.curPos_.isValid();
	bool posvalidbyworldsize = positions_.curPos_.isValidByWorldSize((*field_).size(), (*field_).at(0).size());
	// FIXME mDirection_ is UNDEFINED
	if (proj && posvalid  && posvalidbyworldsize)
	{
		auto next_pos = positions_.curPos_;
		next_pos.stepInDirection();
		auto next_pos_type = (*field_)[next_pos.x_][next_pos.y_].type_;
		if (next_pos_type == BaseGameObject::Type::WALL)
//		||next_pos_type == BaseGameObject::Type::PLAYER )
		{
			field_->at(next_pos.x_).at(next_pos.y_).type_ = BaseGameObject::Type::SPACE;
			field_->at(positions_.prevPos_.x_).at(positions_.prevPos_.y_).type_ = BaseGameObject::Type::SPACE;
			positions_.prevPos_ = positions_.curPos_;
			positions_.curPos_ = next_pos;
			positions_.dPos_ = next_pos - positions_.prevPos_;
			return StepReturn::MEET_WALL;
		}
		positions_.prevPos_ = positions_.curPos_;
		positions_.curPos_ = next_pos;
		positions_.dPos_ = next_pos - positions_.prevPos_;

		if (next_pos_type == BaseGameObject::Type::SPACE ) {
			(*field_)[next_pos.x_][next_pos.y_].type_ = BaseGameObject::Type::PROJECTILE;
			(*field_)[positions_.prevPos_.x_][positions_.prevPos_.y_].type_ = BaseGameObject::Type::SPACE;
			return StepReturn::SUCCESS;
		}
		// TODO экран поражения
		return StepReturn::MEET_PROJECTILE;
	}
	return StepReturn::UNDEFINED_BEHAVIOR;
}

void BaseGameObject::setDestinationPoint(const Position &pos) {
	positions_.dstPos_ = pos;
}
