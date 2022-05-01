//
// Created by true on 2022-04-27.
//

#ifndef SSDL_POSITION_H
#define SSDL_POSITION_H


struct Position {
    int x = -1;
    int y = -1;
    int z = 0;
    void move(int x,int y,int z = 0);
	explicit operator bool() const;
	bool isValid() const;
	Position& operator+=(const Position &other);
	Position& operator+=(const Position &&other);
	Position operator+(const Position &other) const;
	Position operator+(const Position &&other) const;
	Position operator-(const Position &other) const;
	Position operator-(const Position &&other) const;
	Position& operator-=(const Position &other);
	Position& operator-=(const Position &&other);

	Position operator-() const;

	bool operator==(const Position &rhs) const;

	bool operator!=(const Position &rhs) const;

	bool operator<(const Position &rhs) const;

	bool operator>(const Position &rhs) const;

	bool operator<=(const Position &rhs) const;

	bool operator>=(const Position &rhs) const;
	enum class Direction {
		Top,
		Bot,
		Left,
		Right,
		Undefined
	} mDirection;
	void stepInDirection(bool inverse = false);
	void reverseDirection();
};

struct Positions{
	Position prevPos;
	Position curPos;
	Position dPos;
	Position dstPos;
};


#endif //SSDL_POSITION_H
