#include <stdio.h>
#include <iostream>
#include "../Sensor.h"
#include "mock_Controller.h"


int main(void) {
    AbstractController *mock = new AbstractController();

    int write_fds[26];

    for (int i = 0; i < 26; i++) {

        std::cout << "Creating sensor #" << i << "\n";
        Sensor *sensor = new Sensor(mock, 'a' + i, i);
        write_fds[i] = sensor->getWriteFD();

    }

    char buff;
    while (read(STDIN_FILENO, &buff, 1) > 0) {
        for (int i = 0; i < 26; i++)
            if (write(write_fds[i], &buff, 1) != 1) {
                std::cout << "Failed to write to pipe. Bailing!\n";
            }
    }

}
