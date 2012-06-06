#include <stdio.h>
#include <iostream>
#include "../IntersectionController.h"
#include "../Sensor.h"

int tick_fd;

using namespace ControllerInfo;

void *listen(void *args);

static const int PIPES = CONTROLLER_MODE_SENTINEL - SYSTEM_MODE;
int pipe_fds[2];
int write_fds[PIPES];

/*used to test sensors. only based on single intersection, able to trigger all
sensors and mode change operations*/
int main(void)
{
    AbstractController* controller = new IntersectionController(TRAM, "dick");

    /*for each sensor...*/
    for (int i = 0; i < PIPES; i++)
    {

        /*create named pipe to pass sensor trigger data*/
        if (pipe(pipe_fds) != 0)
        {
            std::cout << "Failed to create pipe. Bailing!\n";
            return 1;
        }
        write_fds[i] = pipe_fds[1];

        /*create the sensors and display the keys used to trigger each*/
        if (i < PIPES - 1)
        {
            new Sensor(controller, 'a' + i, i + 1, pipe_fds[0]);

            std::cout << "Press " << char('a' + i) << " for " << 
                          controllerFlagNames[i + 1] << "\n";
        }
        else 
            tick_fd = pipe_fds[0];
    }

    /*create thread to listen for all key presses*/
    pthread_t thread;
    pthread_create(&thread, NULL, listen, NULL);

    controller->tick();
}

/*run by seperate thread to listen for all key presses*/
void *listen(void *args)
{
    char buff;
    while (read(STDIN_FILENO, &buff, 1) > 0)
        for (int i = 0; i < PIPES; i++)
            if (write(write_fds[i], &buff, 1) != 1)
                std::cout << "Failed to write to pipe. Bailing!\n";

    return NULL;
}

