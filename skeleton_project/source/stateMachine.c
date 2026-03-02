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

    if (shouldStop(elev))
    {
        elev->state = STOP;
    }

    if (elevio_stopButton())
    {
        emergencyStop(elev);
        elev->state = STOP;
        return;
    }
    if (floorDefined(elev) && shouldStop(elev))
    {
        elev->state = DOOR_OPEN;
        elevio_motorDirection(DIRN_STOP);
        removeRequest(elev);
    }
    else
    {
        elev->state = STOP;
    }

    if (floorDefined(elev) && elevio_obstruction())
    {
        elev->state = DOOR_OPEN;
    }
};

void stateStop(Elevator *elev)
{
    elev->currentDirection = DIRN_STOP;
    if (shouldStop(elev))
    {
        removeRequest(elev);
        elev->state = DOOR_OPEN;
    }
};

void stateDoorOpen(Elevator *elev)
{
    elev->currentDirection = DIRN_STOP;
    elevio_motorDirection(elev->currentDirection);

    wait3sec();

    // for(int i = 0; i < 3; i++){
    addRequests(elev);
    // elevio_motorDirection(elev->currentDirection);
    // struct timespec ts;
    // ts.tv_sec = 0;
    // ts.tv_nsec = 1000000000; //1000 ms
    // nanosleep(&ts, NULL);
    //}
    removeRequest(elev);

    if (reqIsEmpty(elev))
    {
        elev->state = IDLE;
    }
    else
    {
        elev->state = MOVING;
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
