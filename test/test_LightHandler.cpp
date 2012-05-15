#include "../LightHandler.cpp"
#include <stdio.h>

void printBinary(int subject) {
    for (int i = 0; i < sizeof(int) * 8; i++) {
        printf("%d", (subject << i) & 1);
    }
}

int main(int argc, char **argv) {

    LightHandler *handler = new LightHandler(
            LightHandler::CAR_STRAIGHT | 
            LightHandler::PEDESTRIAN
            );
    printf("Configuration: ");
    printBinary(handler->getConfiguration());
    printf("\n");

    handler->printState();

    handler->setState(bit(light::CAR_STRAIGHT_GO) | bit(light::PEDESTRIAN_STOP), bit(light::PEDESTRIAN_STOP));

    printf("\n\nLights: ");
    printBinary(handler->getLights());
    printf("\n");

    handler->printState();

    return 0;
}
