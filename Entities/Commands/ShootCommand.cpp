//
// Created by true on 2022-05-01.
//

#include "ShootCommand.h"

ShootCommand::ShootCommand(BaseGameObject *object, BaseCommand::Type typo) : BaseCommand(object, typo) {

}

ShootCommand::~ShootCommand() = default;

SdlErrorCodeExample ShootCommand::execute() {
	const auto PREV_POS = obj_->positions_.curPos_;
	const auto PREV_TYPE = (*obj_->field_)[obj_->positions_.curPos_.x_][obj_->positions_.curPos_.y_].type_;
	obj_->step();
	const auto &after_step_type = (*obj_->field_)[obj_->positions_.curPos_.x_][obj_->positions_.curPos_.y_].type_;
	// If we shot not point-blank - lets make projectile obj_, that will move till next wall
	if (after_step_type == BaseGameObject::Type::SPACE) {
		(*obj_->field_)[obj_->positions_.curPos_.x_][obj_->positions_.curPos_.y_].type_ = BaseGameObject::Type::PROJECTILE;
	}
	else if ((after_step_type == BaseGameObject::Type::WALL))
	{
		(*obj_->field_)[obj_->positions_.curPos_.x_][obj_->positions_.curPos_.y_].type_ = BaseGameObject::Type::SPACE;
	}
	if (PREV_TYPE == BaseGameObject::Type::PROJECTILE)
		(*obj_->field_)[PREV_POS.x_][PREV_POS.y_].type_ = BaseGameObject::Type::SPACE;
	return {SdlErrorCodeExample::NO_ERRORS};
}
