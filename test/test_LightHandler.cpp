#include "../LightHandler.cpp"
#include <stdio.h>

using namespace Light;

void printBinary(int subject)
{
    for (int i = 0; i < sizeof(int) * 8; i++)
        printf("%d", (subject << i) & 1);
}

/*used to test the light operation and sequence, triggers two using the same
functions that would be used by the state machine*/
int main(int argc, char **argv)
{
    LightHandler *handler = new LightHandler(CAR_STRAIGHT | PEDESTRIAN);
    printf("Configuration: ");
    printBinary(handler->getConfiguration());
    printf("\n");

    handler->printState();

    handler->setState(bit(CAR_STRAIGHT_GO) | bit(PEDESTRIAN_STOP),
        bit(PEDESTRIAN_STOP));

    printf("\n\nLights: ");
    printBinary(handler->getLights());
    printf("\n");

    handler->printState();

    return 0;
}

