#include <timer.h>
#include "elevio.h"

void wait3sec()
{

    struct timespec ts;
    ts.tv_sec = 3;
    ts.tv_nsec = 0;

    nanosleep(&ts, NULL);
};