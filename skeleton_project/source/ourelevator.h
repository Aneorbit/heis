#pragma once
#include "driver/elevio.h"
#include <stdbool.h>
#include <time.h>


typedef struct
{
    int state;
    int currentFloor;
    MotorDirection currentDirection;
    int requests[N_FLOORS][N_BUTTONS];
    struct timespec doorOpenTime;
    int doorTimerOpen;
    int doorTimerActive;
} Elevator;


void init(Elevator *elev);
bool floorDefined(Elevator *elev);
bool secure(Elevator *elev);
void addRequests(Elevator *elev);
void clearRequests(Elevator *elev);
void emergencyStop(Elevator *elev);
void updateRequests(Elevator *elev);
void removeRequest(Elevator *elev);
void handleRequests(Elevator *elev);
bool reqIsEmpty(Elevator *elev);
bool requestsUnder(Elevator *elev);
bool requestsAbove(Elevator *elev);
void updateOutputs(Elevator *elev);
bool shouldStop(Elevator *elev);
MotorDirection chooseDirection(Elevator *elev);