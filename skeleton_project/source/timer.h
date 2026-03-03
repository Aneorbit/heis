#pragma once
#include "ourelevator.h"
#include <stdbool.h>

void startDoorTimer(Elevator *elev);
int doorTimerExpired(Elevator *elev);
