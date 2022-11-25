//
// Created by true on 2022-04-30.
//


#include <iostream>
#include "MainProcessor.h"

#ifndef MAKE_LOG
MainProcessor::MainProcessor(GameWorld& world):
	world_(world),
	npcProcessor_(world),
    syncStreamErrors_(std::cerr)
{
}
#else
MainProcessor::MainProcessor(GameWorld& world,std::osyncstream &logs):
		syncStreamErrors_(std::cerr),
		logsSynchroStream_(logs),
		world_(world)
{
	particlesSystem_.init();
	for (auto &item: particlesSystem_.projectilesByDistances_) {
		item.init();
	}
}
#endif


void MainProcessor::addCommand(BaseCommand &&command) {
	const std::lock_guard<std::mutex> LOCK(mutexCommands_);
	if (command.type_ == BaseCommand::Type::PLAYER_SHOOT_COMMAND || command.type_ == BaseCommand::Type::NPC_SHOOT_COMMAND)
	{
		shootCommands_.emplace_back(command);
	}
	else if (command.type_ == BaseCommand::Type::PLAYER_MOVE_COMMAND)
	{
		playerMoveCommands_.emplace_back(command);
	}
	else{
		npcMoveCommands_.emplace_back(command);
	}
}

bool MainProcessor::processCommands() {
	bool status = false;
	if (!playerMoveCommands_.empty()) {
		std::vector<BaseCommand> empty;
		{
			const std::lock_guard<std::mutex> LOCK(mutexCommands_);
			std::swap(empty, playerMoveCommands_);
		}
		// Return status tells, need or not rerender screen
		status |= processPlayerMoveCommands(empty);
	}else
	{
		status |= false;
	}
	if (!shootCommands_.empty()) {
		std::vector<BaseCommand> empty;
		{
			const std::lock_guard<std::mutex> LOCK(mutexCommands_);
			std::swap(empty, shootCommands_);
		}
		status |= processShootCommands(empty);
	}
	return status;
}

bool MainProcessor::processProjectilesMoving() {
	auto positions = world_.allProjectilesStep();
	if (!positions.empty())
	{
		for (const auto &position: positions)
		{
			projectilePositions_.emplace_back(position.prevPos_,position.curPos_);
		}

	}
	return true;
}

bool MainProcessor::processShootCommands(std::vector<BaseCommand> &commands) {
	bool status = false;
	for (const auto &command: commands) {
		status |= processShoot(command);
	}
	return status;
}

bool MainProcessor::processPlayerMoveCommands(std::vector<BaseCommand> &commands) {
	if (commands.empty())
		return false;
	processPlayerMove(commands.back());
	return true;
}

// For other commands please create new methods
void MainProcessor::processPlayerMove(const BaseCommand &command) {
	world_.player_.rotate(command.positions_.curPos_.direction_);
	auto s_r = world_.playerStep();
	if (s_r.ret_ == StepReturn::UNDEFINED_BEHAVIOR)
		return;
	Positions POSITIONS;
	POSITIONS = world_.player_.getPositions();
#ifdef DEBUG
	// TODO добавить опцию в cmakelist и в зависимости от нее выставлять define
	// Управление потоком выполнения препятствуют предсказанию потока выполнения в процессоре
	// Поэтому в релизе подобных вещей быть не должно, по крайней мере в критичных
	// К скорости выполнения участках.
	if (command.obj_->type_ != GameObject::Type::PLAYER) {
		syncStreamErrors_ << "Try to stepInDirection non player object\n";
		syncStreamErrors_.emit();
		return false;
	}
	if (command.obj_->getPositions().curPos_ != world_.player_->getPositions().curPos_) {
		syncStreamErrors_ << "Position of player in object is not matching to world->player position\n";
		syncStreamErrors_.emit();
		return false;
	}
#endif
	if (s_r.ret_ == StepReturn::SUCCESS)
	{
		playerMoveChangedPositions_.emplace_back(POSITIONS.prevPos_, POSITIONS.curPos_);
	}
	else
	{
		playerMoveChangedPositions_.emplace_back(POSITIONS.curPos_, POSITIONS.curPos_);
	}
}
// If player press SPACE - lets shoot
bool MainProcessor::processShoot(const BaseCommand &command) {
	Position first_shoot_render_place;
	if (command.positions_.curPos_ != Position{})
		first_shoot_render_place = command.positions_.curPos_;
	else
		first_shoot_render_place = world_.player_.getPositions().curPos_; // take position of player

	first_shoot_render_place.stepInDirection();
	if (world_.addProjectile(first_shoot_render_place))
	{
		projectilePositions_.emplace_back(std::pair{
				first_shoot_render_place,first_shoot_render_place});
		return true;
	}
	return false;
}

RenderMoveInfo MainProcessor::getPlayerChangedPositions() {
	RenderMoveInfo out;
	const std::lock_guard<std::mutex> LOCK(mutexCommands_);
	std::swap(out, playerMoveChangedPositions_);
	return out;
}

RenderMoveInfo MainProcessor::getNpcChangedPositions ()
{
	RenderMoveInfo out;
	const std::lock_guard<std::mutex> LOCK(mutexCommands_);
	std::swap(out, npcMoveChangedPositions_);
	return out;
}

//GameObject MainProcessor::getPlayer() const {
//	return world_.player_;
//}

Position MainProcessor::worldSize() const {
	return world_.size();
}

GameObject::Type MainProcessor::typeAt(const Position &pos) const {
	return world_.typeAt(pos);
}

RenderShootInfo MainProcessor::getShoots()
{
	RenderShootInfo empty;
	const std::lock_guard<std::mutex> LOCK(mutexCommands_);
	std::swap (empty, projectilePositions_);
	return empty;
}

//void MainProcessor::processingLoop ()
//{
//	// обработать все и передать конкретные координаты для обмена в мир для обновления
//	auto positions = allProjectilesStepSecond();
//}

//#include <unordered_set>
//std::vector<Positions> MainProcessor::allProjectilesStepSecond ()
//{
//	std::vector<Positions> output{};
//	std::unordered_set<size_t> explosed;
//	for (size_t i = 0; i < projectiles_.count(); ++i)
//	{
//		auto pos = projectiles_[i];
//		const auto PREV_POS = pos;
//		pos.stepInDirection();
//		const auto D_POS = pos - PREV_POS;
//		auto s_r = projectileStepSecond(projectiles_[i], typeAt(pos),pos);
//		if(s_r.ret_ == StepReturn::MEET_WALL        || s_r.ret_ == StepReturn::MEET_PLAYER
//		|| s_r.ret_ == StepReturn::MEET_PROJECTILE  || s_r.ret_ == StepReturn::OUT_OF_FIELD)
//			explosed.insert(i);
//		else
//			projectiles_[i].stepInDirection();
//		output.emplace_back(Positions{PREV_POS,pos});//,D_POS,{}});
//	}
//	for (const auto EXPLOSE: explosed)
//	{
//		projectiles_.remove(EXPLOSE);
//	}
//	return output;
//}

//StepReturn
//MainProcessor::projectileStepSecond (const Position &prev_pos, GameObject::Type dst_type, const Position &dst_pos)
//{
//	switch (dst_type)
//	{
////		case GameObject::Type::SPACE:	return {StepReturn::SUCCESS, dst_pos};
////		case GameObject::Type::PROJECTILE:	return {StepReturn::MEET_PROJECTILE, dst_pos};
////		case GameObject::Type::WALL: return {StepReturn::MEET_WALL, dst_pos};
////		case GameObject::Type::PLAYER:	return {StepReturn::MEET_PLAYER, dst_pos};
////		default: return {StepReturn::UNDEFINED_BEHAVIOR,{}};
//		case GameObject::Type::SPACE:	return {StepReturn::SUCCESS};
//		case GameObject::Type::PROJECTILE:	return {StepReturn::MEET_PROJECTILE};
//		case GameObject::Type::WALL: return {StepReturn::MEET_WALL};
//		case GameObject::Type::PLAYER:	return {StepReturn::MEET_PLAYER};
//		default: return {StepReturn::UNDEFINED_BEHAVIOR};
//	}
//}

bool MainProcessor::processNpc ()
{
	const auto npc_data = npcProcessor_.step();
	auto shoots = npc_data.NpcShoots;
	for (size_t i  = 0; i < shoots.count();i++)
	{
		addCommand({BaseCommand::Type::NPC_SHOOT_COMMAND,{shoots[i]}});
	}
	auto moves = npc_data.NpcMooves;
	for (size_t i  = 0; i < moves.count();i++)
	{
		addCommand({BaseCommand::Type::NPC_MOVE_COMMAND,{moves[i]}});
	}
	return true;
}
