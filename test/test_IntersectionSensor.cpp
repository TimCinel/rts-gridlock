#include <stdio.h>
#include <iostream>
#include "../IntersectionController.h"
#include "../Sensor.h"

AbstractController *controller;
int tick_fd;


using namespace ControllerInfo;


void *listen(void *args);

    static const int PIPES = CONTROLLER_MODE_SENTINEL - SYSTEM_MODE;
    int pipe_fds[2];
    int write_fds[PIPES];

int main(void) {

    controller = new IntersectionController(TRAM, "dick");

    int i;
    for (i = 0; i < PIPES; i++) {

        if (pipe(pipe_fds) != 0) {
            std::cout << "Failed to create pipe. Bailing!\n";
            return 1;
        }
        write_fds[i] = pipe_fds[1];

        if (i < PIPES - 1)
        {
            new Sensor(controller, 'a' + i, i + 1, pipe_fds[0]);

            std::cout << "Press " << char('a' + i) << " for " << 
                          controllerFlagNames[i + 1] << "\n";
        }
        else 
        {
            tick_fd = pipe_fds[0];
        }

    }

    pthread_t thread;
    pthread_create(&thread, NULL, listen, NULL);

    controller->tick();


}


void *listen(void *args)
{
    char buff;
    while (read(STDIN_FILENO, &buff, 1) > 0) {
        for (int i = 0; i < PIPES; i++)
            if (write(write_fds[i], &buff, 1) != 1) {
                std::cout << "Failed to write to pipe. Bailing!\n";
            }
    }

    return NULL;
}

