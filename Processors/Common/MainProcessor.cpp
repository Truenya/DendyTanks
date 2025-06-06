#include "MainProcessor.h"
#include <atomic>
#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>

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
		if (command.uid == world_.my_uuid ())
			command.positions_ = world_.tanks_[command.uid].getPositions ();
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
	for (const auto &command : commands) {
		processTankMove (command);
	}
	return true;
}

// For other commands please create new methods
void MainProcessor::processTankMove (const BaseCommand &command)
{
	auto &tank = world_.tanks_[command.uid];
	assert (command.positions_.curPos_.direction_ != Position::Direction::EQUAL);
	tank.rotate (command.positions_.curPos_.direction_);
	auto s_r = world_.step (tank.getPositions ());
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
		tank.step ();
		playerChangedPositions_.emplace_back (tank.getPositions ().prevPos_, tank.getPositions ().curPos_);
	}
	else
	{
		playerChangedPositions_.emplace_back (tank.getPositions ().curPos_, tank.getPositions ().curPos_);
	}
}

// If player press SPACE - lets shoot
bool MainProcessor::processShoot (const BaseCommand &command)
{
	Position first_shoot_render_place;
	if (command.uid == world_.my_uuid ())
		first_shoot_render_place = world_.tanks_[GameWorld::my_uuid ()].getPositions ().curPos_;// take position of player
	else
		first_shoot_render_place = command.positions_.curPos_;

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

void MainProcessor::processingNpcLoop (const std::atomic<bool> &working)
{
	while (working.load ()) {
		std::this_thread::sleep_for (std::chrono::milliseconds (500));
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
	}
}

bool MainProcessor::noTankAtPos (const Position pos)
{
	for (const auto &[_, tank] : world_.tanks_) {
		if (tank.getPositions ().curPos_ == pos) return true;
	}
	return false;
}
