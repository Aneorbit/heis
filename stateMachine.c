#include <stateMachine.h>

int updateStateMachine()
{
    switch (state)
    {
    case IDLE:
        break;

    case RUNNING_UP:
        printf("Going up!");
        break;

    case RUNNING_DOWN:
        printf("Going down!");
        break;

    case STOP:
        printf("Stopping!");
    }
};
