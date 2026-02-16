#pragma once

typedef enum
{
    IDLE = 0,
    RUNNING_UP = 1,
    RUNNING_DOWN = 2,
    STOP = 3,
    DOOR_OPEN = 4

} State;

State state;
int updateStateMachine();