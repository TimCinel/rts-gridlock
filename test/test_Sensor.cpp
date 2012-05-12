#include <stdio.h>
#include <iostream>
#include "../Sensor.h"


int main(void) {
    Controller *mock = new Controller();

    int pipe_fds[2];
    int write_fds[26];

    for (int i = 0; i < 26; i++) {

        if (pipe(pipe_fds) != 0) {
            std::cout << "Failed to create pipe. Bailing!\n";
            exit(1);
        }

        write_fds[i] = pipe_fds[1];

        std::cout << "Creating sensor #" << i << "\n";
        Sensor *sensor = new Sensor(mock, 'a' + i, i, pipe_fds[0]);
    }

    char buff;
    while (read(STDIN_FILENO, &buff, 1) > 0) {
        for (int i = 0; i < 26; i++)
            if (write(write_fds[i], &buff, 1) != 1) {
                std::cout << "Failed to write to pipe. Bailing!\n";
            }
    }

}
