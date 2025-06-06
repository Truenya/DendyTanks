#include "GameObject.h"

GameObject::~GameObject () = default;

GameObject::GameObject () :
    type_ (GameObject::Type::UNDEFINED),
    positions_ ({{-1, -1, 0, Position::Direction::BOT},
                 {-1, -1, 0, Position::Direction::BOT}})
{}

const Positions &GameObject::getPositions () const
{
	return positions_;
}

GameObject::GameObject (const Position &pos, GameObject::Type typo) :
    type_ (typo),
    positions_ ({pos, pos})
{}

void GameObject::step ()
{
	positions_.prevPos_ = positions_.curPos_;
	positions_.curPos_.stepInDirection ();
}

void GameObject::rotate (Position::Direction direction)
{
	positions_.curPos_.direction_ = direction;
}
