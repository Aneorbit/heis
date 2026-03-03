#define _POSIX_C_SOURCE 200809L
#include "timer.h"
#include <sys/time.h>
#include <stdlib.h>
#include "driver/elevio.h"

void startDoorTimer(Elevator *elev)
{
    clock_gettime(CLOCK_MONOTONIC, &elev->doorOpenTime);
    elev->doorTimerActive = 1;
}

int doorTimerExpired(Elevator *elev)
{
    if (!elev->doorTimerActive)
        return 0;

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    double elapsed =
        (now.tv_sec - elev->doorOpenTime.tv_sec) +
        (now.tv_nsec - elev->doorOpenTime.tv_nsec) / 1e9;

    return elapsed >= 3.0;
}
