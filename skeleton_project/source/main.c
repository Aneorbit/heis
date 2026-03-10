#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "ourelevator.h"
#include "stateMachine.h"

int main()
{
    Elevator elevator;
    Elevator *elev = &elevator;

    elevio_init();
    init(elev);

    while (1)
    {

        addRequests(elev);

        if (elevio_stopButton())
        {
            emergencyStop(elev);
        }
        StateMachine(elev);
        updateOutputs(elev);

        
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 20000000;
        nanosleep(&ts, NULL);
        
    }

    return 0;

   
}
