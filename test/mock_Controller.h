#include <iostream>
#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
private:
    int sensorFlags;
public:
    Controller() { sensorFlags = 0; }

    void setSensorFlag(int flagPosition) {

        sensorFlags |= 1 << flagPosition;

        std::cout << "\nFlag set: " << flagPosition << "\n";
    };
};

#endif
