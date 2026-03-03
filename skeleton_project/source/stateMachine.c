#include "stateMachine.h"
#include "ourelevator.h"
#include "timer.h"

void stateIdle(Elevator *elev)
{
    if (reqIsEmpty(elev))
    {
        elev->state = IDLE;
    }
    else
    {
        elev->state = MOVING;
    }
}

void stateMoving(Elevator *elev)
{
    elev->currentDirection = chooseDirection(elev);
    elevio_motorDirection(elev->currentDirection);

    if (elevio_stopButton())
    {
        emergencyStop(elev);
        return;
    }
    if (floorDefined(elev) && shouldStop(elev))
    {
        elev->state = DOOR_OPEN;
        elevio_motorDirection(DIRN_STOP);
    }
    else if (!floorDefined(elev) && elev->currentDirection == DIRN_STOP)
    {
        elev->state = IDLE;
    }
};

void stateStop(Elevator *elev)
{
    elev->currentDirection = DIRN_STOP;
    elevio_motorDirection(DIRN_STOP);

    if (!elevio_stopButton())
    {
        elevio_stopLamp(0);

        if (floorDefined(elev))
        {
            elev->state = DOOR_OPEN;
            elev->doorTimerActive = 0;
        }
        else if (!floorDefined(elev))
        {
            elev->state = IDLE;
        }
        {
            // if (reqIsEmpty(elev))
            // {
            //     elev->state = IDLE;
            // }
            // else
            // {
            //     elev->state = MOVING;
            // }
        }
    }

    if (floorDefined(elev) && shouldStop(elev))
    {
        removeRequest(elev);
        elev->state = DOOR_OPEN;
    }
};

void stateDoorOpen(Elevator *elev)
{
    elev->currentDirection = DIRN_STOP;
    elevio_motorDirection(DIRN_STOP);
    removeRequest(elev);

    if (!elev->doorTimerActive)
    {
        startDoorTimer(elev);
        return;
    }

    if (doorTimerExpired(elev))
    {
        elev->doorTimerActive = 0;
        removeRequest(elev);
        if (reqIsEmpty(elev))
        {
            elev->state = IDLE;
        }
        else
        {
            elev->state = MOVING;
        }
    }

    if (elevio_obstruction())
    {
        elev->state = DOOR_OPEN;
    }
};

void StateMachine(Elevator *elev)
{
    switch (elev->state)
    {

    case IDLE:
        stateIdle(elev);
        break;

    case STOP:
        stateStop(elev);
        break;

    case DOOR_OPEN:
        stateDoorOpen(elev);
        break;

    case MOVING:
        stateMoving(elev);
        break;
    }
}
