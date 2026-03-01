#pragma once

typedef enum
{
    IDLE = 0,
    STOP = 1,
    DOOR_OPEN = 2,
    MOVING = 3

} State;

void StateMachine(Elevator *elev);
void stateIdle(Elevator *elev);
void stateMoving(Elevator *elev);
void stateIdle(Elevator *elev);
void stateStop(Elevator *elev);