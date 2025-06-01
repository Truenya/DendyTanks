
#ifndef SSDL_STEPRESULT_H
#define SSDL_STEPRESULT_H

#include "Position.h"

struct StepResult {
    enum class ReturnCode {
        SUCCESS,
        MEET_WALL,
        MEET_PLAYER,
        MEET_ENEMY,
        MEET_PROJECTILE,
        OUT_OF_FIELD,
        UNDEFINED_BEHAVIOR
    };

    StepResult(ReturnCode ret = ReturnCode::UNDEFINED_BEHAVIOR)
        : return_code(ret) {}

    ReturnCode return_code;
    Position previous_position;
};

#endif //SSDL_STEPRESULT_H
