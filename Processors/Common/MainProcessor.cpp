//
// Created by true on 2022-04-30.
//


#include "MainProcessor.h"
#include <iostream>
#include <mutex>

#ifndef MAKE_LOG
MainProcessor::MainProcessor (GameWorld &world) :
    world_ (world),
    npcProcessor_ (world),
    syncStreamErrors_ (std::cerr)
{
}
#else
MainProcessor::MainProcessor (GameWorld &world, std::osyncstream &logs) :
    syncStreamErrors_ (std::cerr),
    logsSynchroStream_ (logs),
    world_ (world)
{
	particlesSystem_.init ();
	for (auto &item : particlesSystem_.projectilesByDistances_) {
		item.init ();
	}
}
#endif


void MainProcessor::addCommand (BaseCommand command)
{
	using enum BaseCommand::Type;
	if (command.type_ == SHOOT_COMMAND)
	{
		if (command.uid == world_.my_uuid())
			command.positions_ = world_.tanks_[command.uid].getPositions();
		const std::scoped_lock LOCK (mutexShoots_);
		shootCommands_.emplace_back (command);
	}
	else if (command.type_ == MOVE_COMMAND)
	{
		const std::scoped_lock LOCK (mutexMoves_);
		moveCommands_.emplace_back (command);
	}
}

void MainProcessor::processCommands ()
{
	if (!moveCommands_.empty ()) {
		std::vector<BaseCommand> empty;
		prepareCommands (empty);
		processMoveCommands (empty);
	}
	if (!shootCommands_.empty ()) {
		std::vector<BaseCommand> empty;
		prepareShoots (empty);
		processShootCommands (empty);
	}

}
void MainProcessor::prepareShoots (std::vector<BaseCommand> &empty)
{
	const std::scoped_lock LOCK (mutexShoots_);
	std::swap (empty, shootCommands_);
}
void MainProcessor::prepareCommands (std::vector<BaseCommand> &empty)
{
	const std::scoped_lock LOCK (mutexMoves_);
	std::swap (empty, moveCommands_);
}

bool MainProcessor::processProjectilesMoving ()
{
	if (auto positions = world_.allProjectilesStep (); !positions.empty ())
	{
		for (const auto &position : positions)
		{
			projectilePositions_.emplace_back (position.prevPos_, position.curPos_);
		}
	}
	return true;
}

bool MainProcessor::processShootCommands (std::vector<BaseCommand> &commands)
{
	bool status = false;
	for (const auto &command : commands) {
		status |= processShoot (command);
	}
	return status;
}

bool MainProcessor::processMoveCommands (std::vector<BaseCommand> &commands)
{
	if (commands.empty ())
		return false;
	processTankMove (commands.back ());
	return true;
}

//bool MainProcessor::processNpcMoveCommands (std::vector<BaseCommand> &commands)
//{
//	if (commands.empty ())
//		return false;
//	processTankMove (commands.back ());
//	return true;
//}

// For other commands please create new methods
//void MainProcessor::processNpcMove (const BaseCommand &command)
//{
//	const size_t size = world_.tanks_.count();
//	for (size_t i = 0; i < size; i++) {
//		auto s_r = world_.step({world_.tanks_[i]});
//		if (s_r.ret_ == StepReturn::UNDEFINED_BEHAVIOR)
//			throw std::runtime_error("[UB] processNpcMove");
//		const auto POSITIONS = world_.player_.getPositions ();
//		if (s_r.ret_ == StepReturn::SUCCESS)
//		{
//			playerChangedPositions_.emplace_back (POSITIONS.prevPos_, POSITIONS.curPos_);
//		} else
//		{
//			// обработка вращения на танка на месте.
//			// отрисовщик не поймет, что надо перерисовать
//			playerChangedPositions_.emplace_back (POSITIONS.curPos_, POSITIONS.curPos_);
//		}
//	}
//}

// For other commands please create new methods
void MainProcessor::processTankMove (const BaseCommand &command)
{
	auto &tank = world_.tanks_[command.uid];
	tank.rotate (command.positions_.curPos_.direction_);
//	world_.player_.rotate (command.positions_.curPos_.direction_);
	auto s_r = world_.step(tank.getPositions());
	if (s_r.ret_ == StepReturn::UNDEFINED_BEHAVIOR)
		return;
#ifdef DEBUG
	// TODO добавить опцию в cmakelist и в зависимости от нее выставлять define
	// Управление потоком выполнения препятствуют предсказанию потока выполнения в процессоре
	// Поэтому в релизе подобных вещей быть не должно, по крайней мере в критичных
	// К скорости выполнения участках.
	if (command.obj_->type_ != GameObject::Type::PLAYER) {
		syncStreamErrors_ << "Try to stepInDirection non player object\n";
		syncStreamErrors_.emit ();
		return false;
	}
	if (command.obj_->getPositions ().curPos_ != world_.player_->getPositions ().curPos_) {
		syncStreamErrors_ << "Position of player in object is not matching to world->player position\n";
		syncStreamErrors_.emit ();
		return false;
	}
#endif
	if (s_r.ret_ == StepReturn::SUCCESS)
	{
//		world_.player_.step();
		tank.step();
		playerChangedPositions_.emplace_back (tank.getPositions().prevPos_, tank.getPositions().curPos_);
//		playerChangedPositions_.emplace_back (world_.player_.getPositions().prevPos_, world_.player_.getPositions().curPos_);
	} else
	{
		playerChangedPositions_.emplace_back (tank.getPositions().curPos_, tank.getPositions().curPos_);
//		playerChangedPositions_.emplace_back (world_.player_.getPositions().curPos_, world_.player_.getPositions().curPos_);
	}
}
// If player press SPACE - lets shoot
bool MainProcessor::processShoot (const BaseCommand &command)
{
	Position first_shoot_render_place;
	if (command.positions_.curPos_ != Position{})
		first_shoot_render_place = command.positions_.curPos_;
	else
		first_shoot_render_place = world_.player_.getPositions ().curPos_;// take position of player

	first_shoot_render_place.stepInDirection ();
	if (world_.addProjectile (first_shoot_render_place))
	{
		projectilePositions_.emplace_back (
		        first_shoot_render_place, first_shoot_render_place);
		return true;
	}
	return false;
}

RenderMoveInfo MainProcessor::getPlayerChangedPositions ()
{
	RenderMoveInfo out;
	const std::scoped_lock LOCK (mutexMoves_);
	std::swap (out, playerChangedPositions_);
	return out;
}

RenderMoveInfo MainProcessor::getNpcChangedPositions ()
{
	RenderMoveInfo out;
	const std::scoped_lock LOCK (mutexMoves_);
	std::swap (out, npcChangedPositions_);
	return out;
}

//GameObject MainProcessor::getPlayer() const {
//	return world_.player_;
//}

Position MainProcessor::worldSize () const
{
	return world_.size ();
}

GameObject::Type MainProcessor::typeAt (const Position &pos) const
{
	return world_.typeAt (pos);
}

RenderShootInfo MainProcessor::getShoots ()
{
	RenderShootInfo empty;
	const std::scoped_lock LOCK (mutexMoves_);
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
	const auto NPC_DATA = npcProcessor_.step ();
	auto shoots = NPC_DATA.NpcShoots;
	for (size_t i = 0; i < shoots.count (); i++)
	{
		addCommand (shoots[i]);
	}
	auto moves = NPC_DATA.NpcMooves;
	for (size_t i = 0; i < moves.count (); i++)
	{
		addCommand (moves[i]);
	}
	return true;
}
