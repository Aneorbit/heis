#include <stateMachine.h>
#include <elevator.h>
#include <timer.h>

void stateIdle(Elevator *elev)
{
    if (reqIsEmpty)
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

    if (shouldStop(elev))
    {
        elev->state = STOP;
    }
    elev->currentDirection = chooseDirection(elev);
    if (elevio_stopButton() && floorDefined() || (elevio_obstruction()))
    {
        elev->state = DOOR_OPEN;
    }
    while (elevio_stopButton() && !floorDefined())
    {
        elev->state = STOP;
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

    while ((elevio_stopButton() && floorDefined()) || elevio_obstruction() && floorDefined())
    {
        elev->state = DOOR_OPEN;
    }

    wait3sec(elev);

    if (reqIsEmpty)
    {
        elev->state = IDLE;
    }
    else
    {
        elev->state = MOVING;
    }
    while (elevio_obstruction())
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
