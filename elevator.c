#include <elevator.h>
#include <elevio.h>
#include <stateMachine.h>

// Elevator struct
struct Elevator
{
    int state;
    int currentFloor;
    int requests[N_FLOORS][N_BUTTONS];
};

bool floorDefined()
{
    return elevio_floorSensor != -1;
}

void init()
{
    if floorDefined ()                // initialisering for å sjekke om vi skal begynne å ta bestillinger. Hvis ikke break eller noe.
        elevio_floorIndicator(floor); // asserter om vi er ved en etasje
}

bool secure()
{
    return (floorDefined() && state == STOP); // urde kanskje legge til om den er innenfor definert område
}
void clearRequests(requests)
{ // må være noe pointers involvert, fikses senere-
    for (int i = 0; i <= N_FLOORS; i++)
    {
        for (int j = 0; j <= N_BUTTONS; j++)
        {
            requests[i][j] = 0;
        }
    }
}

void emergencyStop()
{
    if (elevio_stopButton())
    {
        MotorDirection(DIRN_STOP);
        elevio_buttonLamp(1); // må velge riktig lampe
        clearRequests();

        while (elevio_stopButton())
        {
        }
        elevio_buttonLamp(0); // velge riktig lampe her også?
        state = IDLE;
    }
}

// Tenker å ha en funksjon requestsIsEmpty som sjekker om bestillingsmatrisen er tom, og den sjekkes hele tiden, så hvis vi ikke har
// noen bestillinger settes state til IDEL.
// har en funksjon for å oppdatere matrisen?
// heisen skal uansett snu retning i 1 og 4 etg
// skal kjøre èn retning, og for hver etasje sjekke om matrisen på den etasjen (current floor) e rtom, og hvis noen av knappene er høy skal døren åpnes.
//