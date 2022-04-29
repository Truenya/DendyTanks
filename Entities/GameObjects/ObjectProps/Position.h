//
// Created by true on 2022-04-27.
//

#ifndef SSDL_POSITION_H
#define SSDL_POSITION_H


struct Position {
    int x_;
    int y_;
    int z_ = 0;
    void move(int x,int y,int z = 0);
	bool isValid();
	Position operator+=(const Position &other);
	Position operator+=(const Position &&other);
};


#endif //SSDL_POSITION_H
