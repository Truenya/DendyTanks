//
// Created by true on 2022-04-27.
//

#ifndef SSDL_POSITION_H
#define SSDL_POSITION_H


#include <cstddef>
#include <cstdio>
#include <map>

struct Position {
	ssize_t x_ = 0;
	ssize_t y_ = 0;
	ssize_t z_ = 0;
	//    void move(int x,int y,int z = 0);
	explicit operator bool () const;
	[[nodiscard]] bool isValid () const;
	[[nodiscard]] bool isValidByWorldSize (Position world_size) const;
	Position &operator+= (const Position &other);
	Position &operator+= (const Position &&other);
	Position operator+ (const Position &other) const;
	Position operator+ (const Position &&other) const;
	Position operator- (const Position &other) const;
	Position operator- (const Position &&other) const;
	Position &operator-= (const Position &other);
	Position &operator-= (const Position &&other);

	Position operator- () const;

	bool operator== (const Position &rhs) const;

	bool operator!= (const Position &rhs) const;

	bool operator<(const Position &rhs) const;

	bool operator> (const Position &rhs) const;

	bool operator<= (const Position &rhs) const;

	bool operator>= (const Position &rhs) const;
	enum class Direction
	{
		TOP,
		BOT,
		LEFT,
		RIGHT,
		EQUAL
	} direction_ = Direction::EQUAL;
	typedef std::pair<Direction, Direction> Directions;
	[[nodiscard]] Directions directionsTo  (const Position &other) const;
	bool stepInDirection (bool inverse = false);
	void reverseDirection ();
//	[[nodiscard]] double calcDistance (const Position &other) const;
};


struct Positions {
	Position prevPos_;
	Position curPos_;
//	Position dPos_;
//	Position dstPos_;
};


#endif//SSDL_POSITION_H
