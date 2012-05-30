#include <stdio.h>
#include <iostream>
#include "../IntersectionController.h"
#include "../Sensor.h"

AbstractController *controller;
int tick_fd;


using namespace ControllerInfo;


void *tick_listen(void *args);

int main(void) {
    int pipe_fds[2];
    static const int PIPES = CONTROLLER_MODE_SENTINEL - CMD_EW_STRAIGHT + 1;
    int write_fds[PIPES];

    controller = new IntersectionController();

    int i;
    for (i = 0; i < PIPES; i++) {

        if (pipe(pipe_fds) != 0) {
            std::cout << "Failed to create pipe. Bailing!\n";
            exit(1);
        }
        write_fds[i] = pipe_fds[1];

        if (i < PIPES - 1)
        {
            Sensor *sensor = new Sensor(controller, 'a' + i, i + 1, pipe_fds[0]);

            std::cout << "Press " << char('a' + i) << " for " << 
                          controllerFlagNames[i + 1] << "\n";
        }
        else 
        {
            tick_fd = pipe_fds[0];
            pthread_t thread;
            pthread_create(&thread, NULL, tick_listen, NULL);
        }


    }

    char buff;
    while (read(STDIN_FILENO, &buff, 1) > 0) {
        for (int i = 0; i < PIPES; i++)
            if (write(write_fds[i], &buff, 1) != 1) {
                std::cout << "Failed to write to pipe. Bailing!\n";
            }
    }




}


void *tick_listen(void *args)
{
    char buff;

    std::cout << "Enter \".\" to tick.\n";

    while (read(tick_fd, &buff, 1) > 0) 
        if ('.' == buff) 
            controller->tick();
}

