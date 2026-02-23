#include <elevator.h>
#include <elevio.h>
#include <stateMachine.h>

bool floorDefined()
{
    return elevio_floorSensor != -1;
}

void init()
{
    int floor = elevio_floorSensor();
    if (floorDefined())
    {
        elevio_floorIndicator(floor);
    }
}

bool secure()
{
    return (floorDefined() && state == STOP); // urde kanskje legge til om den er innenfor definert område
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
    for (int i = 0; i <= N_FLOORS; i++)
    {
        for (int j = 0; j <= N_BUTTONS; j++)
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
    return !empty; // returnerer True dersom alle tallene er 0, altså en tom matrise.
}

void emergencyStop(Elevator *elev)
{
    if (elevio_stopButton())
    {
        MotorDirection(DIRN_STOP);
        elevio_stopLamp(1);
        clearRequests(elev);

        while (elevio_stopButton())
        {
        }
        elevio_stopLamp(0);
        state = IDLE;
    }
}

bool requestsAbove(Elevator *elev)
{
    int req = 0;
    int currentFloor = elevio_floorSensor();
    for (int f = currentFloor; f < N_FLOORS; f++)
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
    for (int f = currentFloor; f >= 0; f--)
    {
        for (int b = 0; b < N_BUTTONS; b++)
        {
            req += elev->requests[f][b];
        }
    }
    return req; // returnerer true dersom det er noen bestillinger under
}

// her lager vi to funksjoner, en for om man skal stoppe og en for hvilken retning vi skal
void handleRequests(Elevator *elev)
{
    if (reqIsEmpty(elev))
    {
        elev->state = IDLE;
    }
    elev->currentFloor = elevio_floorSensor();
    elev->currentDirection =

        while (elev->state == RUNNING_UP)
    {
        while (elevio_floorSensor() != -1)
        {
            if (elev->currentFloor == 0)
            {
                if (elev->requests[0][0] == 1 | elev->requests[0][2] == 1)
                {
                    elev->currentDirection = DIRN_STOP;
                    elevio_doorOpenLamp();
                    // stopp i tre sekunder med nanosleep
                }
            }
        }
    }
    while (elev->state == RUNNING_DOWN)
    {
    }

    // switch(currentFloor) {
    //     case 0: //etasje 1

    //         break;

    //     case 1: //etasje 2

    //         break;

    //     case 2: //etasje 3
    //         break;

    //     case 3: //etasje 4
    //         break;
}

while (floorDefined())
{
    for (int b)
}
if ()
}
while ()
}

// Tenker å ha en funksjon requestsIsEmpty som sjekker om bestillingsmatrisen er tom, og den sjekkes hele tiden, så hvis vi ikke har
// noen bestillinger settes state til IDLE.
// har en funksjon for å oppdatere matrisen?
// heisen skal uansett snu retning i 1 og 4 etg
// skal kjøre èn retning, og for hver etasje sjekke om matrisen på den etasjen (current floor) e rtom, og hvis noen av knappene er høy skal døren åpnes.
//