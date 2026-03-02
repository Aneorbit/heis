#include "ourelevator.h"
#include "driver/elevio.h"
#include "stateMachine.h"
#include "timer.h"
#include <stdio.h>

bool floorDefined(Elevator *elev)
{
    return elevio_floorSensor() != -1;
}

void init(Elevator *elev)
{
    int floor = elevio_floorSensor();
    if (!floorDefined(elev))
    {
        elevio_motorDirection(DIRN_DOWN);
    }
    if (floor != 0)
    {
        elevio_motorDirection(DIRN_DOWN);
    }
    while (elevio_floorSensor() != 0)
    {
    }
    elevio_motorDirection(DIRN_STOP);

    elev->currentFloor = 0;
    elev->currentDirection = DIRN_STOP;
    elevio_floorIndicator(0);
    printf("Elevator initialized!");
}

bool secure(Elevator *elev)
{
    return (floorDefined(elev) && elev->currentDirection == DIRN_STOP); // urde kanskje legge til om den er innenfor definert område
}

// Legg til bestilling
void addRequests(Elevator *elev)
{
    for (int f = 0; f < N_FLOORS; f++)
    {
        for (int b = 0; b < N_BUTTONS; b++)
        {
            if (elevio_callButton(f, b))
            {
                elev->requests[f][b] = 1;
            }
        }
    }
}

// fjerner alle bestillinger ved en etasje (etter utført)
void removeRequest(Elevator *elev)
{
    for (int b = 0; b < N_BUTTONS; b++)
    {
        elev->requests[elev->currentFloor][b] = 0;
    }
}

// Tøm hele bestillingsmatrisen
void clearRequests(Elevator *elev)
{
    for (int i = 0; i < N_FLOORS; i++)
    {
        for (int j = 0; j < N_BUTTONS; j++)
        {
            elev->requests[i][j] = 0;
        }
    }
}

bool reqIsEmpty(Elevator *elev)
{
    int empty = 0;
    for (int f = 0; f < N_FLOORS; f++)
    {
        for (int b = 0; b < N_BUTTONS; b++)
        {
            empty += elev->requests[f][b];
        }
    }
    if (empty == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void emergencyStop(Elevator *elev)
{
    if (elevio_stopButton())
    {
        elev->currentDirection = DIRN_STOP;
        elevio_motorDirection(elev->currentDirection);
        elevio_stopLamp(1);
        clearRequests(elev);
    }
    else
    {
        elevio_stopLamp(0);
    }
}

void updateOutputs(Elevator *elev)
{
    int floor = elevio_floorSensor();
    if (floorDefined(elev))
    {
        elev->currentFloor = floor;
        elevio_floorIndicator(floor);
    }

    for (int f = 0; f < N_FLOORS; f++)
    {
        for (int b = 0; b < N_BUTTONS; b++)
        {
            elevio_buttonLamp(f, b, elev->requests[f][b]);
        }
    }

    elevio_stopLamp(elevio_stopButton());
    elevio_doorOpenLamp(elev->state == DOOR_OPEN);
}

bool requestsAbove(Elevator *elev)
{
    int req = 0;
    int currentFloor = elevio_floorSensor();
    for (int f = currentFloor + 1; f < N_FLOORS; f++)
    {
        for (int b = 0; b < N_BUTTONS; b++)
        {
            req += elev->requests[f][b];
        }
    }
    return req; // req = 0 hvis det ikke er flere bestillinger over, og req blir 1 om det er bestillinger over (bool blir true dersom req er 1 ELLER høyere. bare 0 er false)
}

bool requestsUnder(Elevator *elev)
{
    int req = 0;
    int currentFloor = elevio_floorSensor();
    for (int f = currentFloor - 1; f >= 0; f--)
    {
        for (int b = 0; b < N_BUTTONS; b++)
        {
            req += elev->requests[f][b];
        }
    }
    return req > 0; // returnerer true dersom det er noen bestillinger under
}

bool shouldStop(Elevator *elev)
{
    if (floorDefined(elev))
    {
        for (int b = 0; b < N_BUTTONS; b++)
        {
            if (elev->requests[elev->currentFloor][b] == 1)
            {
                return true;
            };
        }
    }
    return false;
}

MotorDirection chooseDirection(Elevator *elev)
{

    // if (elev->currentFloor == 3){
    //     if (requestsUnder)
    //     {
    //         elev->currentDirection = DIRN_DOWN;
    //     }
    //     else
    //     {
    //         elev->currentDirection = DIRN_STOP;
    //     }
    // }

    // if (elev->currentFloor == 0)
    // {
    //     if (requestsAbove)
    //     {
    //         elev->currentDirection = DIRN_UP;if (reqIsEmpty(elev)) {
    //             elev->currentDirection = DIRN_STOP;
    //         }
    //     }
    //     else
    //     {
    //         elev->currentDirection = DIRN_STOP;
    //     }
    // }

    if (floorDefined(elev))
    {
        if (reqIsEmpty(elev))
        {
            elev->currentDirection = DIRN_STOP;
            return DIRN_STOP;
        }
        if (elev->currentDirection == DIRN_STOP)
        {
            if (requestsAbove(elev))
            {
                elev->currentDirection = DIRN_UP;
            }
            if (requestsUnder(elev))
            {
                elev->currentDirection = DIRN_DOWN;
            }
        }

        if (elev->currentDirection == DIRN_UP)
        {
            if (requestsAbove(elev))
            {
                elev->currentDirection = DIRN_UP;
            }
            else if (requestsUnder(elev))
            {
                elev->currentDirection = DIRN_DOWN;
            }
        }

        if (elev->currentDirection == DIRN_DOWN)
        {
            if (requestsUnder(elev))
            {
                elev->currentDirection = DIRN_DOWN;
            }
            else if (requestsAbove(elev))
            {
                elev->currentDirection = DIRN_UP;
            }
        }
    }
    return elev->currentDirection;
}
