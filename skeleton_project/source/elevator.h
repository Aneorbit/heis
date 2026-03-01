#pragma once
#include "driver/elevio.h"
#include <stdbool.h>

// Elevator struct
typedef struct
{
    int state;
    int currentFloor;
    MotorDirection currentDirection;
    int requests[N_FLOORS][N_BUTTONS];
} Elevator;

// Elevator functions
void init();
bool floorDefined();
bool secure();
void clearRequests(Elevator *elev);
bool secure();
void emergencyStop();
void updateRequests(Elevator *elev);
void removeRequest(Elevator *elev);
void handleRequests(Elevator *elev);
bool reqIsEmpty(Elevator *elev);
bool requestsUnder(Elevator *elev);
bool requestsAbove(Elevator *elev);
void updateOutputs(Elevator *elev);
bool shouldStop(Elevator *elev);
MotorDirection chooseDirection(Elevator *elev);