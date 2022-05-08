//
// Created by true on 2022-05-01.
//

#include "ShootCommand.h"

ShootCommand::ShootCommand(BaseGameObject *object, BaseCommand::Type typo) : BaseCommand(object, typo) {

}

ShootCommand::~ShootCommand() = default;

SdlErrorCodeExample ShootCommand::execute() {
	const auto prev_pos = obj_->positions_.curPos_;
	const auto prev_type = (*obj_->field_)[obj_->positions_.curPos_.x_][obj_->positions_.curPos_.y_].type_;
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
	if (prev_type == BaseGameObject::Type::PROJECTILE)
		(*obj_->field_)[prev_pos.x_][prev_pos.y_].type_ = BaseGameObject::Type::SPACE;
	return {SdlErrorCodeExample::NO_ERRORS};
}
